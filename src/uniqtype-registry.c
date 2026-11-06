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
 * names, and also the 1-byte codeless aliases.
 * 
 * The registry is filled from every meta-DSO in the link map, in load order.
 * Meta-DSOs are never unmapped (RTLD_NODELETE),
 * so the entries' names, which point into their dynstrs, stay valid.
 **/

struct registry_entry
{
	uint32_t hash;
	uint32_t len;
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

static uint32_t hash_name(const char *name, uint32_t *out_len)
{
	uint32_t h = 5381;
	const char *p = name;
	for (; *p; ++p) h = h * 33 + (unsigned char) *p;
	*out_len = p - name;
	return h;
}

static struct registry_entry *find_slot(uint32_t h, const char *name, uint32_t len)
{
	unsigned long i = h & (capacity - 1);
	for (;;)
	{
		struct registry_entry *e = &table[i];
		if (!e->name) return e;
		if (e->hash == h && e->len == len && 0 == memcmp(e->name, name, len)) return e;
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
		if (old_table[i].name) *find_slot(old_table[i].hash, old_table[i].name, old_table[i].len) = old_table[i];
	}
	if (old_table) __private_free(old_table);
	return 1;
}

static struct uniqtype *insert(const char *name, struct uniqtype *u)
{
	if (10 * (count + 1) > 7 * capacity && !grow()) return u;
	uint32_t len;
	uint32_t h = hash_name(name, &len);
	struct registry_entry *e = find_slot(h, name, len);
	if (e->name) return e->u;
	*e = (struct registry_entry) { h, len, name, u };
	++count;
	return u;
}

static void scan_object(struct link_map *l)
{
	ElfW(Sym) *dynsym = get_dynsym(l);
	unsigned char *dynstr = get_dynstr(l);
	if (!dynsym || !dynstr) return;
	unsigned long nsyms = dynamic_symbol_count(l->l_ld, l);
	for (unsigned long i = 1; i < nsyms; ++i)
	{
		const char *name = (const char *) dynstr + dynsym[i].st_name;
		if (__liballocs_uniqtype_sym_is_uniqtype(&dynsym[i], name))
		{
			insert(name, (struct uniqtype *) (l->l_addr + dynsym[i].st_value));
		}
	}
}

static _Bool ensure_initialized(void)
{
	if (capacity) return 1;
	if (!grow()) return 0;
	for (struct link_map *l = find_r_debug()->r_map; l; l = l->l_next) scan_object(l);
	return 1;
}

struct uniqtype *__liballocs_uniqtype_registry_register(const char *name, struct uniqtype *u)
{
	if (!ensure_initialized()) return u;
	return insert(name, u);
}

struct uniqtype *__liballocs_uniqtype_registry_lookup(const char *name)
{
	if (!ensure_initialized()) return NULL;
	uint32_t len;
	uint32_t h = hash_name(name, &len);
	struct registry_entry *e = find_slot(h, name, len);
	return e->name ? e->u : NULL;
}
