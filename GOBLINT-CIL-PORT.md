# Porting `lp2` onto upstream's goblint-cil

Investigation from 2026-10-23, against `stephen/master` @ `f1dc713`. Nothing was changed
during the investigation; all merges below were trial merges (`git merge-tree`).

## Summary

`lp2` has **already** moved to goblint-cil: it was merged from master before the
merge-base (`03f7e79`). lp2's committed `contrib/cil` pointer (`5c31f05`, goblint-based)
is a direct ancestor of upstream's (`c472cb6`), so it fast-forwards. A trial merge of
lp2 as committed into upstream has **no** conflict in `contrib/cil` or `contrib/toolsub`.

What conflicts is the **working tree**, which has `contrib/cil` and `contrib/toolsub`
checked out on the old, pre-goblint CIL lineage. The only local CIL commit
(overflow builtins) is redundant on goblint-cil.

## State at time of investigation

| Submodule | lp2 records | working tree | upstream records |
|---|---|---|---|
| `contrib/cil` | `5c31f05` (goblint, mbyzhangAlt fork) | `92db7b8` (old CIL 1.7.3 lineage, branch `fix-overflow-builtins`) | `c472cb6` (`stephenrkell/cil`, branch `goblint-develop+srk`) |
| `contrib/toolsub` | `0aa9d07` (mbyzhang, goblint-compatible) | `4f5c5e8` (older, ancestor of upstream) | `e627432` (`stephenrkell/toolsub`) |
| `contrib/liballocstool` | `257563b` | `3a01414` (branch `fix-type-mischaracterisation`) | `b874132` |
| `contrib/libsystrap` | `fed0fdb` | `ef8383a` ("wip", on `fork/alaska`) | `e40bfa8` |
| `contrib/libsystrap/contrib/librunt` | `fe167de` (in libsystrap) | `a19b930` (on `fork/alaska`) | `1e49ce6` (in upstream libsystrap) |
| `contrib/libmallochooks` | `952ff60` | `952ff60` | `a0c2929` (contains `952ff60`) |

Divergence: lp2 is 19 commits ahead of the merge-base, upstream is 31 ahead.

### Things that exist only on local disk

Every submodule commit is pushed to some remote. Only these are local-only:

- The 3 uncommitted superproject edits: `src/allocators/static-file.c`,
  `src/liballocs_private.h`, `tools/stubgen.h`. Full patch is in the
  [appendix](#appendix-uncommitted-superproject-patch). They merge cleanly with upstream.
- libsystrap's uncommitted librunt pointer bump `fe167de → a19b930`.
- `contrib/cil/_opam/`: a 621 MB local opam switch holding only the old `cil` package.
  It can be rebuilt, and upstream's build no longer uses it.
- `contrib/cil` also has 384 deleted `doc/` files. They are generated output (`.o`, `.tex`,
  `cilcode.tmp/*`), so nothing is lost by restoring them.

## CIL-specific work

### 1. `contrib/cil`: take upstream `c472cb6`, drop `fix-overflow-builtins`

`92db7b8` "chore: add overflow builtins" registers the 18 type-specific
`__builtin_{s,u}{add,sub,mul}{,l,ll}_overflow` as untyped varargs. goblint-cil already has
all 18, with typed signatures, in both `5c31f05` and `c472cb6`. The commit is redundant.

Other old-lineage features were checked against `c472cb6` and are present there:
`__builtin_dynamic_object_size`, `convertvector`, `rotate{left,right}`, `assume_aligned`,
`___stpncpy_chk`, `isinf_sign`, `no_sanitize`, `pure`, `_Thread_local`, `offsetof`,
`_Float64`.

**One small gap** remains from the old-lineage commit `6f3fe35c` ("fix: dynamic object size
builtin missing"). goblint-cil registers `__builtin_dynamic_object_size`, but
`src/frontc/cabs2cil.ml` only special-cases `__builtin_object_size` for side-effecting
arguments (folding the call to -1 or 0). The old fix was:

```ocaml
else if fv.vname = "__builtin_object_size" || fv.vname = "__builtin_dynamic_object_size" then begin
```

If you rely on this, it's a 2-line PR to `goblint-develop+srk`.

### 2. `contrib/toolsub`: take upstream `e627432`

The working tree `4f5c5e8` is a strict ancestor of upstream's pointer and has no local
commits. The only untracked files are `cilpp/src` build products.

### 3. `tools/lang/c/trapptrwrites/trapptrwrites.ml:121-124`: must fix

This is broken on lp2 as committed, independent of the submodule question.

| | `Set` | `Call` |
|---|---|---|
| goblint-cil | `lval * exp * location * location` | `lval option * exp * exp list * location * location` |
| old CIL | `lval * exp * location` | `lval option * exp * exp list * location` |

The memcpy/memmove hunk added on lp2 uses old CIL's 4-field `Call`. The rest of the file
uses goblint's 5-field form. As committed, the file compiles against **neither** CIL, and
`trapptrwrites` is in the default `CIL_TOOLS` in `tools/lang/c/Makefile`. The fix:

```ocaml
| Call (ret, (Lval (Var f, NoOffset) as fexp), ([dest; src; n] as args), l, el)
    when isMemCopyName f.vname ->
    ChangeTo [ Call (ret, fexp, args, l, el);
               Call (None, Lval (var notifyCopyFun), [dest; src; n], l, el) ]
```

This was found by comparing the type definitions; it has not been compiled.

### 4. `tests/Makefile` conflict: drop `-g3`, keep DWARF 4

Upstream commit `8ae30f1` removed `-g3` on purpose: "Temporarily avoid -g3 and -Wp,-dD
while we lack certain CIL changes on the goblint-develop+srk branch". The old
"grokking #defines via -Wp,-dMD" CIL commit was reverted in the goblint line. lp2's
`e66de9b` has `-g3 -gdwarf-4`. Resolve as:

```make
CFLAGS += -g -gdwarf-4 -gstrict-dwarf -std=c99 -fno-eliminate-unused-debug-types
```

Upstream did the same for `tools/allocscompilerwrapper.py` (`8ae30f1`) and
`tests/alloca-clang/mk.inc` (`e8eeba7`). Both auto-merge.

### 5. Build integration (arrives through the merge, but the local environment needs updating)

| | lp2 | upstream |
|---|---|---|
| CIL build | `opam switch create .` in `contrib/cil`, then `opam install . --destdir=...` | `cd contrib/cil && dune build @install` |
| Variable | `CIL_INSTALL` | `CIL_LIBDIR` = `contrib/cil/_build/install/default/lib/goblint-cil` |
| configure flag | `--with-cil-install` | `--with-cil-libdir` |
| toolsub | `make -C toolsub ...` | `USE_GOBLINT=1 make -C toolsub ...` |

lp2 never touched `contrib/Makefile`, and upstream's `allocscc` change (`280f219`, heed
`CIL_LIBDIR`) auto-merges with lp2's `ALLOCSCC_TRAP_PTR_WRITES` hunk. The local
environment still needs:

- **Rerun `./autogen.sh && ./configure`.** The current `config.mk` has
  `CIL_INSTALL := /home/zoltan/Develop/stackscan/contrib/liballocs/contrib/cil/lib`, which
  is a different checkout. `LIBALLOCS_OCAML_PREFIX` is empty.
- **Install goblint-cil's dependencies into the ambient OCaml switch** (currently
  `~/.opam/4.14.2`, which has no CIL packages):
  `opam install --deps-only ./contrib/cil` (uses `goblint-cil.opam`). Upstream `dacdda8`
  restores OCaml 4.13 support, so 4.14.2 should be fine.
- Afterwards `contrib/cil/_opam/` is dead weight and can be deleted. goblint-cil's
  `.gitignore` already ignores it.

## Rest of the merge (not CIL-specific)

Trial merge of lp2 + uncommitted work into `stephen/master`. The content conflicts are
`.circleci/build-dependencies.txt`, `.circleci/config.yml`, `src/allocsites.c` and
`tests/Makefile`, plus all the moved submodules.

| Item | Resolution |
|---|---|
| `src/allocsites.c` | Both sides fixed the same wild-allocsite NULL dereference (lp2 `bbbf069`, upstream `a4ed6f3`). Take either side, but in `__liballocs_allocsite_id` keep lp2's `return (allocsite_id_t) -1;`. Upstream returns `NULL` from a function returning `allocsite_id_t`. |
| `.circleci/*` | Take upstream's. It's the newer CI work, merged via PRs #196/#198. |
| libmallochooks | Fast-forward to `a0c2929`, which already contains lp2's `952ff60`. The trial-merge conflict came from missing local objects. |
| liballocstool | 4 local commits are not upstream: `7ea0707` realpath leak, `b8fe191` out-params, `07fbaa1` void return, `3a01414` gitignore. Rebasing or merging onto `b874132` is textually clean. Consider PRing them upstream. |
| libsystrap | 2 local commits (`fed0fdb` Alaska SIGILL, `ef8383a` wip) onto `e40bfa8` is textually clean. Upstream `32bea7b` changed the API for call-based instrumentation, so build and test the Alaska changes. Commit the librunt bump inside libsystrap first. |
| librunt (nested) | `a19b930` (RTLD_NEXT) onto upstream `1e49ce6` is textually clean. Upstream split `felf.h` out of `relf.h`, so build-test this too. |
| Superproject WIP edits | `static-file.c`, `liballocs_private.h` and `stubgen.h` all auto-merge. The `stubgen.h` edit is a real bug fix (missing `\` continuation inside a macro). |
| `configure.ac` | lp2's lifetime-policies change auto-merges. A stale `#undef LIFETIME_INSERT_TYPE` remains in `include/liballocs_config.h.in`. It's harmless and predates this port. |

### Housekeeping before upstreaming

lp2's "wip" commit `6d0730e` committed build output. Drop it before any PR:
`tools/dumpsyscalls` (11 MB), `tools/dwarf-machine-self-test`,
`tests/anon-aliases/{anon-aliases,lib1.s,lib2.s,lib3.s}`, `tests/hello-build-id/hello-build-id`,
`tests/hello-via-wrapper/{hello-via-wrapper,hello-via-wrapper.s}` and `allocsld/allocsld.so.map`.

## Suggested procedure

1. **Secure the WIP.** In the superproject run `git add src tools` and commit. Do **not**
   stage the `contrib/cil` / `contrib/toolsub` pointers. Inside `contrib/libsystrap`, commit
   the librunt bump.
2. **Branch off lp2** and `git merge stephen/master`. Resolve as described above.
3. **Prepare cil for checkout:** `git -C contrib/cil checkout -- doc`. Without this,
   `git submodule update` will likely refuse to switch, because of the deleted generated
   docs. The `fix-overflow-builtins` branch stays in the cil repo and on `fork`.
4. `git submodule sync --recursive && git submodule update --init --recursive`.
5. Fix `trapptrwrites.ml`, install the goblint-cil deps, rerun autogen/configure, then build
   and run the tests.

## Useful commands

```sh
# Recompute the trial merge without touching anything
git fetch stephen
git merge-tree --write-tree --name-only lp2 stephen/master

# Does goblint-cil have builtin X?
git -C contrib/cil grep -n '__builtin_X' c472cb67 -- src
```

## Appendix: uncommitted superproject patch

Captured 2026-10-23 with `git diff -- src tools` on lp2 @ `e78a96d`.

```diff
diff --git a/src/allocators/static-file.c b/src/allocators/static-file.c
index fbb5809..7294118 100644
--- a/src/allocators/static-file.c
+++ b/src/allocators/static-file.c
@@ -306,6 +306,19 @@ struct big_allocation *plug_hole_or_abort(
 		- (uintptr_t) lowest_containing_mapping_bigalloc->end;
 	char *failure_kind = NULL;
 	if (hole_size > BIGGEST_SANE_USER_ALLOC) { failure_kind = "huge hole"; goto hole_err; }
+	struct mapping_sequence *lower_seq = (struct mapping_sequence *)
+		lowest_containing_mapping_bigalloc->allocator_private;
+	/* Degenerate case seen with very large executables (node): the DSO's
+	 * mappings landed in two bigallocs that abut EXACTLY, so there is no
+	 * hole to plug -- a zero-length mmap would fail with EINVAL. Skip the
+	 * plug and go straight to coalescing the two bigallocs. */
+	if (hole_size == 0)
+	{
+		debug_printf(0, "Mappings of DSO `%s' abut exactly (zero-size hole); "
+			"coalescing without a plug\n", l->l_name);
+	}
+	else
+	{
 	debug_printf(0, "Hole is a workable size (0x%lx), so checking the hole remains free...\n",
 		(unsigned long) hole_size);
 	_Bool hole_free = 1;
@@ -344,8 +357,6 @@ struct big_allocation *plug_hole_or_abort(
 	assert(plug_mapping_bigalloc);
 	assert(plug_mapping_bigalloc == lowest_containing_mapping_bigalloc);
 #endif
-	struct mapping_sequence *lower_seq = (struct mapping_sequence *)
-		lowest_containing_mapping_bigalloc->allocator_private;
 	_Bool did_augment = __augment_mapping_sequence(lower_seq,
 		lowest_containing_mapping_bigalloc->end,
 		highest_containing_mapping_bigalloc->begin,
@@ -355,6 +366,7 @@ struct big_allocation *plug_hole_or_abort(
 		(char*) lower_seq->filename,
 		plug_hole_or_abort);
 	if (!did_augment) { failure_kind = "augmenting mapping sequence"; goto hole_err; }
+	} /* end hole_size != 0 */
 	/* Now we want to coalesce the two now-abutting bigallocs and fix up the mapping
 	 * sequence metadata. We
 	 * - grab the mapping_sequence pointer from the higher-addressed bgialloc
diff --git a/src/liballocs_private.h b/src/liballocs_private.h
index 356ad09..100a1d4 100644
--- a/src/liballocs_private.h
+++ b/src/liballocs_private.h
@@ -75,7 +75,7 @@ const char *format_symbolic_address(const void *addr);
 #include "pageindex.h"
 
 /* FIXME: 'mappings' should probably be a flexible array member. */
-#define MAPPING_SEQUENCE_MAX_LEN 8
+#define MAPPING_SEQUENCE_MAX_LEN 32 /* was 8; node maps >8 VMAs */
 struct mapping_sequence
 {
 	void *begin;
diff --git a/tools/stubgen.h b/tools/stubgen.h
index 2b7c943..4275444 100644
--- a/tools/stubgen.h
+++ b/tools/stubgen.h
@@ -353,7 +353,7 @@ ALLOC_EVENT(post_successful_alloc)(void *allocptr, size_t modified_size, size_t
 		allocptr /* == userptr */, requested_size, \
 		__current_allocsite ? __current_allocsite : caller, sizefn); \
 	/* Alaska-backed chunks (handles: bit 63 set, hence "negative" pointers) \
-	 * must NOT get the default MANUAL policy */
+	 * must NOT get the default MANUAL policy */ \
 	if (initial_lifetime_policies /* always statically known but we can't #ifdef here */ \
 			&& !((intptr_t) allocptr < 0)) \
 	{ \
```
