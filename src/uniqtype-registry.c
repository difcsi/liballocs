#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <link.h>
#include "relf.h"
#include "liballocs.h"
#include "liballocs_private.h"

/* The registry of canonical uniqtypes.
 *
 * A uniqtype is one object for the whole process, but every meta-DSO
 * defines its own copy of each type it mentions (symbol __uniqtype_<code>_<name>,
 * where the code summarises the type's structure). The canonical copy is the
 * first one loaded; the others are duplicates, and src/meta-dso-canon.c rewrites
 * the pointers a meta-DSO holds so that they all lead to canonical copies.
 *
 * This registry says which copy is canonical, keyed by symbol name: the codeful
 * names, and also the 1-byte codeless aliases (__uniqtype__<name>) that point
 * at them, because a type whose code cannot be computed (say, a pointer to an
 * incomplete struct) is defined under its codeless name in one meta-DSO while
 * another may alias that same name to a codeful definition; a global symbol
 * lookup would unify those, so we must too.
 * It is filled from every object in the link map the first time it is needed
 * (the executable, its libraries, any meta-DSO already loaded) and then from
 * each meta-DSO as it is loaded, in load order, so "first registered" is "first
 * loaded" -- the same definition a global symbol lookup (dlsym RTLD_DEFAULT)
 * finds, since meta-DSOs are loaded RTLD_GLOBAL. Objects dlopened later that
 * are not meta-DSOs are not scanned, which only matters if such an object
 * defines uniqtypes of its own. Meta-DSOs are never unmapped (RTLD_NODELETE),
 * so the entries' names, which point into their dynstrs, stay valid.
 *
 * Keys are the GNU hash of the name with its low bit cleared, which is what
 * an object's .gnu.hash chain array stores for each symbol (the low bit marks
 * the end of a bucket's run). So an object's uniqtype symbols are registered
 * by walking that array (__liballocs_uniqtype_for_each_symbol), without
 * hashing any name or scanning the whole dynsym; an object with no GNU hash
 * table gets a dynsym scan with dl_new_hash instead.
 *
 * Open addressing with linear probing; no libc malloc, since we run while
 * metadata is being loaded. */

struct registry_entry
{
	uint32_t hash; /* the GNU hash of the name, low bit cleared (see below) */
	const char *name; /* in the defining object's dynstr */
	struct uniqtype *u;
};
static struct registry_entry *table;
static unsigned long capacity; /* a power of two, or 0 before first use */
static unsigned long count;

/* A uniqtype symbol: a definition, or a 1-byte alias of one. */
_Bool __liballocs_uniqtype_sym_is_uniqtype(ElfW(Sym) *s, const char *name)
{
	return ELF64_ST_TYPE(s->st_info) == STT_OBJECT
		&& s->st_shndx != SHN_UNDEF
		&& s->st_size > 0
		&& 0 == strncmp(name, "__uniqtype_", sizeof "__uniqtype_" - 1);
}
_Bool __liballocs_uniqtype_sym_is_definition(ElfW(Sym) *s, const char *name)
{
	return __liballocs_uniqtype_sym_is_uniqtype(s, name)
		&& s->st_size > 1 /* aliases are 1 byte long */;
}

/* Keys are the GNU hash (dl_new_hash) of the name with its low bit cleared:
 * that is what an object's .gnu.hash chain array stores for each symbol (the
 * low bit there marks the end of a chain), so a caller walking that array can
 * hand us the hash without touching the name. Equal keys are confirmed by
 * comparing the names, as ld.so does. */
#define KEY_OF(gnu_hash) ((uint32_t) (gnu_hash) & ~1u)

/* The entry for this name, or the empty slot where it would go. */
static struct registry_entry *find_slot(uint32_t key, const char *name)
{
	unsigned long i = key & (capacity - 1);
	for (;;)
	{
		struct registry_entry *e = &table[i];
		if (!e->name) return e;
		if (e->hash == key && 0 == strcmp(e->name, name)) return e;
		i = (i + 1) & (capacity - 1);
	}
}

static _Bool grow(void)
{
	unsigned long new_capacity = capacity ? 2 * capacity : 4096;
	struct registry_entry *new_table = __private_malloc(new_capacity * sizeof (struct registry_entry));
	if (!new_table) return 0;
	memset(new_table, 0, new_capacity * sizeof (struct registry_entry));
	struct registry_entry *old_table = table;
	unsigned long old_capacity = capacity;
	table = new_table;
	capacity = new_capacity;
	for (unsigned long i = 0; i < old_capacity; ++i)
	{
		if (old_table[i].name) *find_slot(old_table[i].hash, old_table[i].name) = old_table[i];
	}
	if (old_table) __private_free(old_table);
	return 1;
}

/* Register this definition unless the name is taken; return the canonical one. */
static struct uniqtype *insert(const char *name, uint32_t key, struct uniqtype *u)
{
	if (10 * (count + 1) > 7 * capacity && !grow()) return u;
	struct registry_entry *e = find_slot(key, name);
	if (e->name) return e->u;
	*e = (struct registry_entry) { key, name, u };
	++count;
	return u;
}

int __liballocs_uniqtype_for_each_symbol(struct link_map *l, uniqtype_sym_cb *cb, void *arg)
{
	ElfW(Sym) *dynsym = get_dynsym(l);
	unsigned char *dynstr = get_dynstr(l);
	if (!dynsym || !dynstr) return 0;
	ElfW(Word) *gnu_hash = get_gnu_hash(l);
	if (gnu_hash)
	{
		/* Header: nbuckets, symoffset, maskwords, shift2; then maskwords
		 * 64-bit bloom words; then the buckets; then one chain word per
		 * symbol from symoffset up, holding that symbol's hash with the
		 * low bit marking the end of a bucket's run. Symbols are sorted by
		 * bucket, so the last word of the last run is the last symbol. */
		uint32_t nbuckets = gnu_hash[0], symoffset = gnu_hash[1], maskwords = gnu_hash[2];
		const uint32_t *buckets = (const uint32_t *) ((const ElfW(Addr) *) &gnu_hash[4] + maskwords);
		const uint32_t *chain = buckets + nbuckets;
		uint32_t last = 0;
		for (uint32_t b = 0; b < nbuckets; ++b)
		{
			uint32_t i = buckets[b];
			if (i < symoffset) continue; /* empty bucket (0) */
			while (!(chain[i - symoffset] & 1u)) ++i;
			if (i > last) last = i;
		}
		for (uint32_t i = symoffset; last != 0 && i <= last; ++i)
		{
			const char *name = (const char *) dynstr + dynsym[i].st_name;
			if (!__liballocs_uniqtype_sym_is_uniqtype(&dynsym[i], name)) continue;
			int ret = cb(&dynsym[i], name, chain[i - symoffset] & ~1u, arg);
			if (ret) return ret;
		}
		return 0;
	}
	/* No GNU hash table: scan the symbol table and hash the names ourselves. */
	unsigned long nsyms = dynamic_symbol_count(l->l_ld, l);
	for (unsigned long i = 1; i < nsyms; ++i)
	{
		const char *name = (const char *) dynstr + dynsym[i].st_name;
		if (!__liballocs_uniqtype_sym_is_uniqtype(&dynsym[i], name)) continue;
		int ret = cb(&dynsym[i], name, KEY_OF(dl_new_hash(name)), arg);
		if (ret) return ret;
	}
	return 0;
}

static int insert_symbol(ElfW(Sym) *s, const char *name, uint32_t hash, void *arg)
{
	struct link_map *l = arg;
	insert(name, KEY_OF(hash), (struct uniqtype *) (l->l_addr + s->st_value));
	return 0;
}
static void scan_object(struct link_map *l)
{
	__liballocs_uniqtype_for_each_symbol(l, insert_symbol, l);
}

static _Bool ensure_initialized(void)
{
	if (capacity) return 1;
	if (!grow()) return 0;
	/* Everything loaded so far, in load order. */
	for (struct link_map *l = find_r_debug()->r_map; l; l = l->l_next) scan_object(l);
	return 1;
}

struct uniqtype *__liballocs_uniqtype_registry_register_hashed(const char *name,
	uint32_t gnu_hash, struct uniqtype *u)
{
	if (!ensure_initialized()) return u;
	return insert(name, KEY_OF(gnu_hash), u);
}

struct uniqtype *__liballocs_uniqtype_registry_register(const char *name, struct uniqtype *u)
{
	return __liballocs_uniqtype_registry_register_hashed(name, dl_new_hash(name), u);
}

struct uniqtype *__liballocs_uniqtype_registry_lookup(const char *name)
{
	if (!ensure_initialized()) return NULL;
	struct registry_entry *e = find_slot(KEY_OF(dl_new_hash(name)), name);
	return e->name ? e->u : NULL;
}
