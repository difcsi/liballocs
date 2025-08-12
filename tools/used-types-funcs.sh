#!/bin/bash

this_filename () {
    # look through the defined functions
    ctr=0
    while true; do
        if [[ -z "${FUNCNAME[$ctr]}" ]]; then
            echo "Error: couldn't find this_filename" 1>&2
            exit 1
        fi
        if [[ "${FUNCNAME[$ctr]}" == "this_filename" ]]; then
            echo ${BASH_SOURCE[$ctr]}
            exit 0
        fi
        ctr=$(( $ctr + 1 ))
    done
}

LIBALLOCS="${LIBALLOCS:-$( dirname "$(this_filename)" )/..}"
LIBALLOCSTOOL="${LIBALLOCSTOOL:-$( dirname "$(this_filename)" )/../contrib/liballocstool}"
USEDTYPES=${USEDTYPES:-${LIBALLOCS}/tools/usedtypes}
CC=${CC:-$(which cc)}
LD=${LD:-$(which ld)}
OBJCOPY=${OBJCOPY:-$(which objcopy)}

# HACK: Seems that clang cannot compile generated files, so let the user choose
# another compiler for these with an environnement variable
META_CC=${META_CC:-${CC}}

compile () {
   src="$1"
   dest="$2"
   asm="$( mktemp --suffix=.s )"
   ${META_CC} -I"${LIBALLOCSTOOL}"/include -S -o "$asm" -x c "$src" && \
   ${META_CC} -c -o "$dest" "$asm" && \
   echo "Compiler generated $dest" 1>&2
}

link_defining_aliases () {
  our_objfile="$1"
  our_usedtypes_obj="$2"
  temporary_out=$( mktemp )
  # NOTE: we used to add aliases here...
  # `nm -fposix "${our_usedtypes_obj}" | $(dirname ${USEDTYPES})/alias-linker-opts-for-base-types.sh | sed -r 's/-Wl,--defsym,/--defsym /g'`
  # but this seems wrong (and, at least, will create "multiple definition" errors at link time)
  #cp "$our_objfile" "$our_objfile".orig.o
  #echo ${LD} -o "$temporary_out" -r "$our_objfile" "$our_usedtypes_obj" "$LIBALLOCS"/tools/libroottypes.a && \
  ${LD} -o "$temporary_out" -r "$our_objfile" "$our_usedtypes_obj" "$LIBALLOCS"/tools/libroottypes.a && \
  echo "Linker generated ${temporary_out}, moving to ${our_objfile}" 1>&2 && \
  mv "$temporary_out" "$our_objfile"
}

symbol_redefinitions () {
    f="$1"
    # Here we are renaming codeless symnames with codeful ones, for the codeful
    # ones that are defined in our temporary (usedtypes) object file.
    # PROBLEM: with bitfields, we can get multiple entries with the same trailing name
    # but different codes. We should *either* avoid defining any alias in those cases,
    # *or* prevent usedtypes from generating the "__uniqtype_01234567_int" alias.
    # We take the former approach here. The 21- and 20-character options to sort and
    # uniq refer to the length of the prefix "__uniqtype_........_".
    nm -fposix --defined-only "$f" | tr -s '[:blank:]' '\t' | cut -f1 | \
      egrep '__uniqtype_([0-9a-f]{8})_' | grep -v '_subobj_names$' | \
      sort -k1.21 | uniq -s20 -c | while read count sym; do \
      case "$count" in (1) echo "$sym";; (*);; esac; done | \
      sed -r 's/__uniqtype_([0-9a-f]{8})_(.*)/--redefine-sym __uniqtype__\2=__uniqtype_\1_\2/'
}

objcopy_and_redefine_codeless_names () {
    our_objfile="$1"
    our_usedtypes_obj="$2"
    
    # now, fill in the codeful names for codeless ones
    second_redefinition_args="$( symbol_redefinitions "$our_usedtypes_obj" )" && \
    echo ${OBJCOPY} $second_redefinition_args "$our_objfile" 1>&2 && \
    ${OBJCOPY} $second_redefinition_args "$our_objfile" && \
    echo "objcopy renamed symbols in $our_objfile according to $second_redefinition_args" 1>&2
}
