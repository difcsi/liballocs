#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <link.h>
#include <dlfcn.h>
#include "librunt.h"
#include "relf.h"
#include "liballocs.h"
#include "liballocs_private.h"
#include "allocmeta.h"

/* Meta-DSOs are linked -Bsymbolic (tools/Makefile.meta), so a uniqtype's
 * references to uniqtypes in the same meta-DSO bind to that meta-DSO's own
 * copies, where ld.so used to bind every reference to the first definition
 * of the symbol in global scope -- which is what made structurally identical
 * types (same summary code, hence same codeful symbol name) one object across
 * meta-DSOs. We restore that here: after loading a meta-DSO we find which of
 * its uniqtype definitions are not the first in link-map order (executable,
 * its libraries, earlier meta-DSOs...), and rewrite every uniqtype pointer the
 * meta-DSO holds -- in its uniqtypes' related[] entries, its allocsites and
 * frame_vaddrs tables, and its metavector records -- to the canonical object.
 * Duplicate objects stay in place (dlsym on the meta-DSO still finds them) but
 * everything they point to is canonical too. */

struct canon_pair { struct uniqtype *own; struct uniqtype *canonical; };

/* In-place heapsort by 'own': we must not call malloc (so no qsort) while
 * loading metadata, and the map can hold thousands of entries. */
static void sift_down(struct canon_pair *a, unsigned root, unsigned end)
{
	while (2 * root + 1 <= end)
	{
		unsigned child = 2 * root + 1, largest = root;
		if (a[largest].own < a[child].own) largest = child;
		if (child + 1 <= end && a[largest].own < a[child + 1].own) largest = child + 1;
		if (largest == root) return;
		struct canon_pair t = a[root]; a[root] = a[largest]; a[largest] = t;
		root = largest;
	}
}
static void sort_pairs(struct canon_pair *a, unsigned n)
{
	if (n < 2) return;
	for (unsigned start = (n - 2) / 2; ; --start)
	{
		sift_down(a, start, n - 1);
		if (start == 0) break;
	}
	for (unsigned end = n - 1; end > 0; --end)
	{
		struct canon_pair t = a[0]; a[0] = a[end]; a[end] = t;
		sift_down(a, 0, end - 1);
	}
}
static struct uniqtype *canonical_of(struct canon_pair *map, unsigned n, struct uniqtype *p)
{
	unsigned lo = 0, hi = n;
	while (lo < hi)
	{
		unsigned mid = lo + (hi - lo) / 2;
		if (map[mid].own < p) lo = mid + 1;
		else if (map[mid].own > p) hi = mid;
		else return map[mid].canonical;
	}
	return p;
}

static _Bool is_uniqtype_definition(ElfW(Sym) *s, const char *name)
{
	return ELF64_ST_TYPE(s->st_info) == STT_OBJECT
		&& s->st_shndx != SHN_UNDEF
		&& s->st_size > 1 /* aliases are 1 byte long */
		&& 0 == strncmp(name, "__uniqtype_", sizeof "__uniqtype_" - 1);
}

static void canonicalise_uniqtype(struct uniqtype *u, ElfW(Sym) *s,
	struct canon_pair *map, unsigned nmap)
{
	if (s->st_size < offsetof(struct uniqtype, related)) return;
	unsigned nrel = (s->st_size - offsetof(struct uniqtype, related))
		/ sizeof (struct uniqtype_rel_info);
	unsigned kind = UNIQTYPE_KIND(u);
	for (unsigned i = 0; i < nrel; ++i)
	{
		/* Entries that are not uniqtype pointers: a composite's names vector
		 * (after its members) and an enumeration's enumerators (after [0],
		 * the base type). Member entries keep the pointer first, like 't'. */
		if (kind == COMPOSITE && i >= u->un.composite.nmemb) continue;
		if (kind == ENUMERATION && i > 0) continue;
		u->related[i].un.t.ptr = canonical_of(map, nmap, u->related[i].un.t.ptr);
	}
}

void canonicalise_meta_dso(struct link_map *l)
{
	ElfW(Sym) *dynsym = get_dynsym(l);
	unsigned char *dynstr = get_dynstr(l);
	unsigned long nsyms = dynamic_symbol_count(l->l_ld, l);
	if (!dynsym || !dynstr || nsyms == 0) return;

	struct canon_pair *map = __private_malloc(nsyms * sizeof (struct canon_pair));
	if (!map) return;
	unsigned nmap = 0, ndefs = 0;
	for (unsigned long i = 1; i < nsyms; ++i)
	{
		ElfW(Sym) *s = &dynsym[i];
		const char *name = (const char *) dynstr + s->st_name;
		if (!is_uniqtype_definition(s, name)) continue;
		++ndefs;
		struct uniqtype *own = (struct uniqtype *) (l->l_addr + s->st_value);
		void *canonical = fake_dlsym(RTLD_DEFAULT, name);
		if (!canonical || canonical == (void*) -1 || canonical == own) continue;
		map[nmap++] = (struct canon_pair) { own, canonical };
	}
	if (nmap > 0)
	{
		sort_pairs(map, nmap);
		for (unsigned long i = 1; i < nsyms; ++i)
		{
			ElfW(Sym) *s = &dynsym[i];
			const char *name = (const char *) dynstr + s->st_name;
			if (is_uniqtype_definition(s, name))
			{
				canonicalise_uniqtype((struct uniqtype *) (l->l_addr + s->st_value), s, map, nmap);
			}
			else if (0 == strcmp(name, "allocsites") && s->st_shndx != SHN_UNDEF)
			{
				struct allocsite_entry *e = (void*) (l->l_addr + s->st_value);
				for (unsigned j = 0; j < s->st_size / sizeof *e; ++j)
				{
					e[j].uniqtype = canonical_of(map, nmap, e[j].uniqtype);
				}
			}
			else if (0 == strcmp(name, "frame_vaddrs") && s->st_shndx != SHN_UNDEF)
			{
				struct frame_allocsite_entry *e = (void*) (l->l_addr + s->st_value);
				for (unsigned j = 0; j < s->st_size / sizeof *e; ++j)
				{
					e[j].entry.uniqtype = canonical_of(map, nmap, e[j].entry.uniqtype);
				}
			}
			else if (0 == strncmp(name, "metavec_0x", sizeof "metavec_0x" - 1)
				&& s->st_shndx != SHN_UNDEF)
			{
				/* Packed records; the uniqtype pointer is a 44-bit field with
				 * the low 3 bits dropped (see vaddr_from_rec for the reloc test). */
				union sym_or_reloc_rec *rec = (void*) (l->l_addr + s->st_value);
				for (unsigned j = 0; j < s->st_size / sizeof *rec; ++j)
				{
					if (rec[j].is_reloc) continue;
					uintptr_t bits = rec[j].sym.uniqtype_ptr_bits_no_lowbits;
					if (!bits) continue;
					struct uniqtype *t = (struct uniqtype *) (bits << 3);
					struct uniqtype *c = canonical_of(map, nmap, t);
					if (c != t) rec[j].sym.uniqtype_ptr_bits_no_lowbits = ((uintptr_t) c) >> 3;
				}
			}
		}
	}
	debug_printf(3, "canonicalised meta-DSO %s: %u of %u uniqtype definitions were duplicates\n",
		l->l_name, nmap, ndefs);
	__private_free(map);
}
