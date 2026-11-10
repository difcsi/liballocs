/* Unit test for src/uniqtype-registry.c: keys are the GNU hash with the low
 * bit masked, confirmed by the name; first registered wins; and the walker
 * over an object's uniqtype symbols hands out the hash the object's .gnu.hash
 * stores. Built by this directory's Makefile as a single-file program. */
#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* What the registry takes from the rest of liballocs. */
void *__private_malloc(size_t n) { return malloc(n); }
void __private_free(void *p) { free(p); }

#include "../../src/uniqtype-registry.c"

static struct uniqtype a, b, c, d;

static void test_low_bit_of_hash_does_not_matter(void)
{
	/* A name whose true GNU hash is odd. */
	static char odd[64];
	for (int i = 0; ; ++i)
	{
		snprintf(odd, sizeof odd, "__uniqtype_00000001_lowbit%d", i);
		if (dl_new_hash(odd) & 1u) break;
	}
	assert((dl_new_hash(odd) & 1u) == 1u);
	/* Registered under the masked hash, as the chain array stores it... */
	assert(__liballocs_uniqtype_registry_register_hashed(odd, dl_new_hash(odd) & ~1u, &a) == &a);
	/* ... found by a lookup that hashes the string itself... */
	assert(__liballocs_uniqtype_registry_lookup(odd) == &a);
	/* ... and the first registered wins. */
	assert(__liballocs_uniqtype_registry_register(odd, &b) == &a);
}

static void test_same_hash_different_name_are_distinct(void)
{
	const char *x = "__uniqtype_aaaaaaaa_x", *y = "__uniqtype_bbbbbbbb_y";
	uint32_t hx = dl_new_hash(x) & ~1u;
	assert(__liballocs_uniqtype_registry_register_hashed(x, hx, &c) == &c);
	assert(__liballocs_uniqtype_registry_register_hashed(y, hx, &d) == &d); /* forced collision */
	assert(__liballocs_uniqtype_registry_register_hashed(x, hx, &a) == &c); /* still distinct, still first */
	assert(__liballocs_uniqtype_registry_register_hashed(y, hx, &a) == &d);
}

/* A fake object: a dynsym of four symbols (null, a non-uniqtype object, a
 * uniqtype definition, its 1-byte alias), their string table, a one-bucket
 * .gnu.hash whose chain holds each hashed symbol's hash (end marked by the
 * low bit), a SysV .hash with the same symbol count, and a dynamic section
 * naming one or the other. Load address 0, so values are absolute. */
#define FAKE_NSYMS 4
struct fake_object
{
	ElfW(Sym) syms[FAKE_NSYMS];
	char strtab[64];
	uint32_t gnu[4 + 2 /* one 64-bit bloom word */ + 1 /* bucket */ + (FAKE_NSYMS - 1) /* chain */];
	ElfW(Word) sysv[2 + 1 + FAKE_NSYMS];
	ElfW(Dyn) dyn[5];
	struct link_map lm;
};
static struct uniqtype fake_def, fake_other;
static const char *fake_names[FAKE_NSYMS] = { "", "other", "__uniqtype_12345678_t", "__uniqtype__t" };

static void build_fake(struct fake_object *f, int use_gnu_hash)
{
	memset(f, 0, sizeof *f);
	unsigned off = 1; /* strtab[0] is the empty name */
	for (int i = 1; i < FAKE_NSYMS; ++i)
	{
		f->syms[i].st_name = off;
		strcpy(f->strtab + off, fake_names[i]);
		off += strlen(fake_names[i]) + 1;
		f->syms[i].st_info = ELF64_ST_INFO(STB_GLOBAL, STT_OBJECT);
		f->syms[i].st_shndx = 1;
	}
	f->syms[1].st_value = (ElfW(Addr)) &fake_other; f->syms[1].st_size = 8;
	f->syms[2].st_value = (ElfW(Addr)) &fake_def;   f->syms[2].st_size = 24;
	f->syms[3].st_value = (ElfW(Addr)) &fake_def;   f->syms[3].st_size = 1;
	/* .gnu.hash: nbuckets, symoffset, maskwords, shift2; bloom; buckets; chain */
	f->gnu[0] = 1; f->gnu[1] = 1; f->gnu[2] = 1; f->gnu[3] = 0;
	f->gnu[4] = ~0u; f->gnu[5] = ~0u;
	f->gnu[6] = 1; /* bucket 0 starts at symbol 1 */
	for (int i = 1; i < FAKE_NSYMS; ++i)
	{
		f->gnu[7 + i - 1] = (dl_new_hash(fake_names[i]) & ~1u) | (i == FAKE_NSYMS - 1 ? 1u : 0u);
	}
	/* .hash: nbucket, nchain, buckets, chains (content irrelevant here) */
	f->sysv[0] = 1; f->sysv[1] = FAKE_NSYMS;
	int n = 0;
	f->dyn[n++] = (ElfW(Dyn)) { DT_SYMTAB, { (ElfW(Addr)) f->syms } };
	f->dyn[n++] = (ElfW(Dyn)) { DT_STRTAB, { (ElfW(Addr)) f->strtab } };
	f->dyn[n++] = (ElfW(Dyn)) { DT_STRSZ, { off } };
	f->dyn[n++] = use_gnu_hash ? (ElfW(Dyn)) { DT_GNU_HASH, { (ElfW(Addr)) f->gnu } }
	                           : (ElfW(Dyn)) { DT_HASH, { (ElfW(Addr)) f->sysv } };
	f->dyn[n++] = (ElfW(Dyn)) { DT_NULL, { 0 } };
	f->lm.l_addr = 0;
	f->lm.l_ld = f->dyn;
}

struct seen { const char *names[8]; uint32_t hashes[8]; ElfW(Sym) *syms[8]; int n; };
static int record(ElfW(Sym) *s, const char *name, uint32_t hash, void *arg)
{
	struct seen *z = arg;
	z->syms[z->n] = s; z->names[z->n] = name; z->hashes[z->n] = hash; ++z->n;
	return 0;
}
static int stop_at_first(ElfW(Sym) *s, const char *name, uint32_t hash, void *arg)
{
	++*(int *) arg;
	return 7;
}

static void test_walker_sees_uniqtype_symbols_with_stored_hash(void)
{
	static struct fake_object f;
	for (int use_gnu_hash = 1; use_gnu_hash >= 0; --use_gnu_hash)
	{
		build_fake(&f, use_gnu_hash);
		struct seen z = { .n = 0 };
		assert(__liballocs_uniqtype_for_each_symbol(&f.lm, record, &z) == 0);
		assert(z.n == 2);
		assert(z.syms[0] == &f.syms[2] && 0 == strcmp(z.names[0], "__uniqtype_12345678_t"));
		assert(z.syms[1] == &f.syms[3] && 0 == strcmp(z.names[1], "__uniqtype__t"));
		assert(z.hashes[0] == (dl_new_hash("__uniqtype_12345678_t") & ~1u));
		assert(z.hashes[1] == (dl_new_hash("__uniqtype__t") & ~1u));
		int calls = 0;
		assert(__liballocs_uniqtype_for_each_symbol(&f.lm, stop_at_first, &calls) == 7);
		assert(calls == 1);
	}
}

/* Growth: more names than the registry's initial capacity holds at its load
 * factor, all registered through the walker, all found afterwards. */
#define BIG_N 3000
static int register_each(ElfW(Sym) *s, const char *name, uint32_t hash, void *arg)
{
	struct link_map *l = arg;
	struct uniqtype *own = (struct uniqtype *) (l->l_addr + s->st_value);
	assert(__liballocs_uniqtype_registry_register_hashed(name, hash, own) == own);
	return 0;
}
static void test_registry_grows_during_a_walk(void)
{
	static ElfW(Sym) syms[BIG_N + 1];
	static char strtab[BIG_N * 32];
	static uint32_t gnu[4 + 2 + 1 + BIG_N];
	static struct uniqtype objs[BIG_N];
	static ElfW(Dyn) dyn[5];
	static struct link_map lm;
	unsigned off = 1;
	gnu[0] = 1; gnu[1] = 1; gnu[2] = 1; gnu[3] = 0; gnu[4] = ~0u; gnu[5] = ~0u; gnu[6] = 1;
	for (int i = 1; i <= BIG_N; ++i)
	{
		syms[i].st_name = off;
		off += sprintf(strtab + off, "__uniqtype_%08x_g%d", (unsigned) i * 2654435761u, i) + 1;
		syms[i].st_info = ELF64_ST_INFO(STB_GLOBAL, STT_OBJECT);
		syms[i].st_shndx = 1;
		syms[i].st_value = (ElfW(Addr)) &objs[i - 1];
		syms[i].st_size = 8;
		gnu[7 + i - 1] = (dl_new_hash(strtab + syms[i].st_name) & ~1u) | (i == BIG_N ? 1u : 0u);
	}
	dyn[0] = (ElfW(Dyn)) { DT_SYMTAB, { (ElfW(Addr)) syms } };
	dyn[1] = (ElfW(Dyn)) { DT_STRTAB, { (ElfW(Addr)) strtab } };
	dyn[2] = (ElfW(Dyn)) { DT_STRSZ, { off } };
	dyn[3] = (ElfW(Dyn)) { DT_GNU_HASH, { (ElfW(Addr)) gnu } };
	dyn[4] = (ElfW(Dyn)) { DT_NULL, { 0 } };
	lm.l_addr = 0; lm.l_ld = dyn;
	assert(__liballocs_uniqtype_for_each_symbol(&lm, register_each, &lm) == 0);
	for (int i = 1; i <= BIG_N; ++i)
	{
		assert(__liballocs_uniqtype_registry_lookup(strtab + syms[i].st_name) == &objs[i - 1]);
	}
}

int main(void)
{
	test_low_bit_of_hash_does_not_matter();
	test_same_hash_different_name_are_distinct();
	test_walker_sees_uniqtype_symbols_with_stored_hash();
	test_registry_grows_during_a_walk();
	printf("uniqtype-registry: ok\n");
	return 0;
}
