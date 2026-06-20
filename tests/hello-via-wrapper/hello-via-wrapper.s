	.file	"hello-via-wrapper.i"
	.text
.Ltext0:
	.file 1 "hello-via-wrapper.i"
#APP
	.pushsection .allocs_srcallocs,"a",@progbits
	.ascii "/home/zoltan/Develop/stackscan/contrib/liballocs/tests/hello-via-wrapper/hello-via-wrapper.c\t10\t10\tmalloc\t__uniqtype____uninterpreted_byte\t1\n"
	.popsection

	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Hello, via wrapper! Got %p\n"
#NO_APP
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB42:
	.file 2 "hello-via-wrapper.c"
	.loc 2 9 1
	.cfi_startproc
	endbr64
	.loc 2 10 3
	.loc 2 11 3
	.loc 2 10 3
	.loc 2 9 1 is_stmt 0
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	.loc 2 10 9
	movl	$42, %edi
	call	malloc@PLT
.LVL0:
.LBB4:
.LBB5:
	.file 3 "/usr/include/x86_64-linux-gnu/bits/stdio2.h"
	.loc 3 118 9
	leaq	.LC0(%rip), %rsi
	movl	$2, %edi
.LBE5:
.LBE4:
	.loc 2 10 9
	movq	%rax, %rbx
.LVL1:
	.loc 2 10 3 is_stmt 1 discriminator 1
	.loc 2 11 3
.LBB7:
.LBB6:
	.loc 3 118 3
	.loc 3 118 3
	.loc 3 118 9 is_stmt 0
	movq	%rax, %rdx
	xorl	%eax, %eax
.LVL2:
	call	__printf_chk@PLT
.LVL3:
	.loc 3 118 3 is_stmt 1 discriminator 1
.LBE6:
.LBE7:
	.loc 2 12 3
	movq	%rbx, %rdi
	call	free@PLT
.LVL4:
	.loc 2 13 3
	.loc 2 15 1 is_stmt 0
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
.LVL5:
	ret
	.cfi_endproc
.LFE42:
	.size	main, .-main
	.text
.Letext0:
	.file 4 "/home/zoltan/Develop/stackscan/contrib/liballocs/include/malloc-meta.h"
	.file 5 "/home/zoltan/Develop/stackscan/contrib/liballocs/include/liballocs_cil_inlines.h"
	.file 6 "/usr/lib/gcc/x86_64-linux-gnu/15/include/stddef.h"
	.file 7 "/usr/include/stdlib.h"
	.file 8 "/usr/lib/gcc/x86_64-linux-gnu/15/include/stdarg.h"
	.file 9 "<built-in>"
	.file 10 "/usr/include/x86_64-linux-gnu/bits/types.h"
	.file 11 "/usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h"
	.file 12 "/usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h"
	.file 13 "/usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h"
	.file 14 "/usr/include/x86_64-linux-gnu/bits/types/__FILE.h"
	.file 15 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h"
	.file 16 "/usr/include/x86_64-linux-gnu/bits/types/FILE.h"
	.file 17 "/usr/include/stdio.h"
	.file 18 "/usr/include/x86_64-linux-gnu/bits/stdio2-decl.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0xb38
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF185
	.byte	0xc
	.long	.LASF186
	.long	.LASF187
	.long	.Ldebug_ranges0+0x30
	.quad	0
	.long	.Ldebug_line0
	.long	.Ldebug_macinfo0
	.uleb128 0x2
	.long	.LASF7
	.byte	0x8
	.byte	0x4
	.byte	0x43
	.byte	0x8
	.long	0x7b
	.uleb128 0x3
	.long	.LASF0
	.byte	0x4
	.byte	0x44
	.byte	0x12
	.long	0x7b
	.byte	0x1
	.byte	0x1
	.byte	0x7
	.byte	0
	.uleb128 0x3
	.long	.LASF1
	.byte	0x4
	.byte	0x45
	.byte	0x12
	.long	0x82
	.byte	0x8
	.byte	0x2f
	.byte	0x10
	.byte	0
	.uleb128 0x3
	.long	.LASF2
	.byte	0x4
	.byte	0x46
	.byte	0x13
	.long	0x89
	.byte	0x2
	.byte	0xc
	.byte	0x4
	.byte	0x6
	.uleb128 0x3
	.long	.LASF3
	.byte	0x4
	.byte	0x47
	.byte	0x12
	.long	0x7b
	.byte	0x1
	.byte	0x4
	.byte	0
	.byte	0x7
	.byte	0
	.uleb128 0x4
	.byte	0x1
	.byte	0x8
	.long	.LASF4
	.uleb128 0x4
	.byte	0x8
	.byte	0x7
	.long	.LASF5
	.uleb128 0x4
	.byte	0x2
	.byte	0x7
	.long	.LASF6
	.uleb128 0x5
	.long	0x89
	.uleb128 0x2
	.long	.LASF8
	.byte	0x8
	.byte	0x4
	.byte	0x43
	.byte	0x8
	.long	0xe3
	.uleb128 0x3
	.long	.LASF9
	.byte	0x4
	.byte	0x44
	.byte	0x12
	.long	0x7b
	.byte	0x1
	.byte	0x1
	.byte	0x7
	.byte	0
	.uleb128 0x3
	.long	.LASF10
	.byte	0x4
	.byte	0x45
	.byte	0xa
	.long	0xe3
	.byte	0x2
	.byte	0xf
	.byte	0
	.byte	0
	.uleb128 0x3
	.long	.LASF11
	.byte	0x4
	.byte	0x46
	.byte	0x12
	.long	0x82
	.byte	0x8
	.byte	0x2c
	.byte	0x4
	.byte	0
	.uleb128 0x3
	.long	.LASF3
	.byte	0x4
	.byte	0x47
	.byte	0x12
	.long	0x7b
	.byte	0x1
	.byte	0x4
	.byte	0
	.byte	0x7
	.byte	0
	.uleb128 0x4
	.byte	0x2
	.byte	0x5
	.long	.LASF12
	.uleb128 0x2
	.long	.LASF13
	.byte	0x8
	.byte	0x4
	.byte	0x43
	.byte	0x8
	.long	0x118
	.uleb128 0x3
	.long	.LASF14
	.byte	0x4
	.byte	0x44
	.byte	0x12
	.long	0x82
	.byte	0x8
	.byte	0x3c
	.byte	0x4
	.byte	0
	.uleb128 0x3
	.long	.LASF3
	.byte	0x4
	.byte	0x45
	.byte	0x12
	.long	0x7b
	.byte	0x1
	.byte	0x4
	.byte	0
	.byte	0x7
	.byte	0
	.uleb128 0x6
	.long	.LASF122
	.byte	0x8
	.byte	0x4
	.byte	0x43
	.byte	0x7
	.long	0x14a
	.uleb128 0x7
	.long	.LASF15
	.byte	0x4
	.byte	0x44
	.byte	0x1a
	.long	0x2d
	.uleb128 0x7
	.long	.LASF16
	.byte	0x4
	.byte	0x45
	.byte	0x1c
	.long	0x95
	.uleb128 0x7
	.long	.LASF17
	.byte	0x4
	.byte	0x46
	.byte	0x19
	.long	0xea
	.byte	0
	.uleb128 0x2
	.long	.LASF18
	.byte	0x8
	.byte	0x4
	.byte	0x43
	.byte	0x8
	.long	0x165
	.uleb128 0x8
	.long	.LASF22
	.byte	0x4
	.byte	0x44
	.byte	0x37
	.long	0x118
	.byte	0
	.byte	0
	.uleb128 0x9
	.long	.LASF19
	.byte	0x4
	.byte	0x63
	.byte	0x17
	.long	0x171
	.uleb128 0xa
	.long	0x82
	.long	0x180
	.uleb128 0xb
	.long	0x180
	.byte	0
	.uleb128 0xc
	.byte	0x8
	.uleb128 0x9
	.long	.LASF20
	.byte	0x4
	.byte	0x85
	.byte	0x17
	.long	0x14a
	.uleb128 0x2
	.long	.LASF21
	.byte	0x40
	.byte	0x5
	.byte	0xbd
	.byte	0x8
	.long	0x1f7
	.uleb128 0x8
	.long	.LASF23
	.byte	0x5
	.byte	0xbe
	.byte	0x12
	.long	0x1f7
	.byte	0
	.uleb128 0x8
	.long	.LASF24
	.byte	0x5
	.byte	0xbf
	.byte	0x12
	.long	0x1f7
	.byte	0x8
	.uleb128 0x8
	.long	.LASF25
	.byte	0x5
	.byte	0xc0
	.byte	0x15
	.long	0x203
	.byte	0x10
	.uleb128 0x8
	.long	.LASF26
	.byte	0x5
	.byte	0xc1
	.byte	0x11
	.long	0x209
	.byte	0x18
	.uleb128 0x8
	.long	.LASF27
	.byte	0x5
	.byte	0xc2
	.byte	0xa
	.long	0xe3
	.byte	0x1c
	.uleb128 0x8
	.long	.LASF28
	.byte	0x5
	.byte	0xc3
	.byte	0x12
	.long	0x7b
	.byte	0x1e
	.uleb128 0x8
	.long	.LASF29
	.byte	0x5
	.byte	0xc4
	.byte	0x12
	.long	0x7b
	.byte	0x1f
	.byte	0
	.uleb128 0xd
	.byte	0x8
	.long	0x1fd
	.uleb128 0xe
	.uleb128 0xf
	.long	.LASF25
	.uleb128 0xd
	.byte	0x8
	.long	0x1fe
	.uleb128 0x4
	.byte	0x4
	.byte	0x7
	.long	.LASF30
	.uleb128 0x10
	.long	.LASF31
	.value	0x280
	.byte	0x5
	.byte	0xcd
	.byte	0x8
	.long	0x26d
	.uleb128 0x8
	.long	.LASF32
	.byte	0x5
	.byte	0xce
	.byte	0x11
	.long	0x209
	.byte	0
	.uleb128 0x8
	.long	.LASF33
	.byte	0x5
	.byte	0xcf
	.byte	0x1b
	.long	0x90
	.byte	0x4
	.uleb128 0x8
	.long	.LASF34
	.byte	0x5
	.byte	0xd0
	.byte	0x13
	.long	0x89
	.byte	0x6
	.uleb128 0x8
	.long	.LASF35
	.byte	0x5
	.byte	0xd1
	.byte	0x12
	.long	0x7b
	.byte	0x8
	.uleb128 0x8
	.long	.LASF36
	.byte	0x5
	.byte	0xd2
	.byte	0x12
	.long	0x7b
	.byte	0x9
	.uleb128 0x8
	.long	.LASF37
	.byte	0x5
	.byte	0xd3
	.byte	0x2e
	.long	0x26d
	.byte	0x40
	.byte	0
	.uleb128 0x11
	.long	0x18e
	.long	0x27d
	.uleb128 0x12
	.long	0x82
	.byte	0x8
	.byte	0
	.uleb128 0x9
	.long	.LASF38
	.byte	0x6
	.byte	0xe5
	.byte	0x17
	.long	0x82
	.uleb128 0x13
	.long	.LASF39
	.byte	0x6
	.value	0x158
	.byte	0xd
	.long	0x296
	.uleb128 0x14
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.long	.LASF40
	.byte	0x8
	.byte	0x7
	.byte	0x3f
	.byte	0x8
	.long	0x2c5
	.uleb128 0x8
	.long	.LASF41
	.byte	0x7
	.byte	0x40
	.byte	0x8
	.long	0x296
	.byte	0
	.uleb128 0x15
	.string	"rem"
	.byte	0x7
	.byte	0x41
	.byte	0x8
	.long	0x296
	.byte	0x4
	.byte	0
	.uleb128 0x9
	.long	.LASF42
	.byte	0x7
	.byte	0x3f
	.byte	0x2d
	.long	0x29d
	.uleb128 0x2
	.long	.LASF43
	.byte	0x10
	.byte	0x7
	.byte	0x47
	.byte	0x8
	.long	0x2f9
	.uleb128 0x8
	.long	.LASF41
	.byte	0x7
	.byte	0x48
	.byte	0x9
	.long	0x2f9
	.byte	0
	.uleb128 0x15
	.string	"rem"
	.byte	0x7
	.byte	0x49
	.byte	0x9
	.long	0x2f9
	.byte	0x8
	.byte	0
	.uleb128 0x4
	.byte	0x8
	.byte	0x5
	.long	.LASF44
	.uleb128 0x9
	.long	.LASF45
	.byte	0x7
	.byte	0x47
	.byte	0x2e
	.long	0x2d1
	.uleb128 0x2
	.long	.LASF46
	.byte	0x10
	.byte	0x7
	.byte	0x52
	.byte	0x8
	.long	0x334
	.uleb128 0x8
	.long	.LASF41
	.byte	0x7
	.byte	0x53
	.byte	0xe
	.long	0x334
	.byte	0
	.uleb128 0x15
	.string	"rem"
	.byte	0x7
	.byte	0x54
	.byte	0xe
	.long	0x334
	.byte	0x8
	.byte	0
	.uleb128 0x4
	.byte	0x8
	.byte	0x5
	.long	.LASF47
	.uleb128 0x9
	.long	.LASF48
	.byte	0x7
	.byte	0x52
	.byte	0x30
	.long	0x30c
	.uleb128 0x13
	.long	.LASF49
	.byte	0x7
	.value	0x3cb
	.byte	0xf
	.long	0x354
	.uleb128 0xd
	.byte	0x8
	.long	0x35a
	.uleb128 0xa
	.long	0x296
	.long	0x36e
	.uleb128 0xb
	.long	0x1f7
	.uleb128 0xb
	.long	0x1f7
	.byte	0
	.uleb128 0x9
	.long	.LASF50
	.byte	0x8
	.byte	0x29
	.byte	0x1b
	.long	0x37a
	.uleb128 0x16
	.long	.LASF188
	.long	0x383
	.uleb128 0x11
	.long	0x393
	.long	0x393
	.uleb128 0x12
	.long	0x82
	.byte	0
	.byte	0
	.uleb128 0x17
	.long	.LASF189
	.byte	0x18
	.byte	0x9
	.byte	0
	.long	0x3d0
	.uleb128 0x18
	.long	.LASF51
	.byte	0x9
	.byte	0
	.long	0x209
	.byte	0
	.uleb128 0x18
	.long	.LASF52
	.byte	0x9
	.byte	0
	.long	0x209
	.byte	0x4
	.uleb128 0x18
	.long	.LASF53
	.byte	0x9
	.byte	0
	.long	0x180
	.byte	0x8
	.uleb128 0x18
	.long	.LASF54
	.byte	0x9
	.byte	0
	.long	0x180
	.byte	0x10
	.byte	0
	.uleb128 0x9
	.long	.LASF55
	.byte	0xa
	.byte	0x1f
	.byte	0x17
	.long	0x7b
	.uleb128 0x9
	.long	.LASF56
	.byte	0xa
	.byte	0x20
	.byte	0x18
	.long	0x89
	.uleb128 0x9
	.long	.LASF57
	.byte	0xa
	.byte	0x21
	.byte	0x16
	.long	0x209
	.uleb128 0x9
	.long	.LASF58
	.byte	0xa
	.byte	0x22
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF59
	.byte	0xa
	.byte	0x25
	.byte	0x15
	.long	0x40c
	.uleb128 0x4
	.byte	0x1
	.byte	0x6
	.long	.LASF60
	.uleb128 0x9
	.long	.LASF61
	.byte	0xa
	.byte	0x26
	.byte	0x17
	.long	0x7b
	.uleb128 0x9
	.long	.LASF62
	.byte	0xa
	.byte	0x27
	.byte	0xf
	.long	0xe3
	.uleb128 0x9
	.long	.LASF63
	.byte	0xa
	.byte	0x28
	.byte	0x18
	.long	0x89
	.uleb128 0x9
	.long	.LASF64
	.byte	0xa
	.byte	0x29
	.byte	0xd
	.long	0x296
	.uleb128 0x9
	.long	.LASF65
	.byte	0xa
	.byte	0x2a
	.byte	0x16
	.long	0x209
	.uleb128 0x9
	.long	.LASF66
	.byte	0xa
	.byte	0x2c
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF67
	.byte	0xa
	.byte	0x2d
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF68
	.byte	0xa
	.byte	0x34
	.byte	0x12
	.long	0x400
	.uleb128 0x9
	.long	.LASF69
	.byte	0xa
	.byte	0x35
	.byte	0x13
	.long	0x413
	.uleb128 0x9
	.long	.LASF70
	.byte	0xa
	.byte	0x36
	.byte	0x13
	.long	0x41f
	.uleb128 0x9
	.long	.LASF71
	.byte	0xa
	.byte	0x37
	.byte	0x14
	.long	0x42b
	.uleb128 0x9
	.long	.LASF72
	.byte	0xa
	.byte	0x38
	.byte	0x13
	.long	0x437
	.uleb128 0x9
	.long	.LASF73
	.byte	0xa
	.byte	0x39
	.byte	0x14
	.long	0x443
	.uleb128 0x9
	.long	.LASF74
	.byte	0xa
	.byte	0x3a
	.byte	0x13
	.long	0x44f
	.uleb128 0x9
	.long	.LASF75
	.byte	0xa
	.byte	0x3b
	.byte	0x14
	.long	0x45b
	.uleb128 0x9
	.long	.LASF76
	.byte	0xa
	.byte	0x3f
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF77
	.byte	0xa
	.byte	0x40
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF78
	.byte	0xa
	.byte	0x48
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF79
	.byte	0xa
	.byte	0x49
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF80
	.byte	0xa
	.byte	0x91
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF81
	.byte	0xa
	.byte	0x92
	.byte	0x16
	.long	0x209
	.uleb128 0x9
	.long	.LASF82
	.byte	0xa
	.byte	0x93
	.byte	0x16
	.long	0x209
	.uleb128 0x9
	.long	.LASF83
	.byte	0xa
	.byte	0x94
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF84
	.byte	0xa
	.byte	0x95
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF85
	.byte	0xa
	.byte	0x96
	.byte	0x16
	.long	0x209
	.uleb128 0x9
	.long	.LASF86
	.byte	0xa
	.byte	0x97
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF87
	.byte	0xa
	.byte	0x98
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF88
	.byte	0xa
	.byte	0x99
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF89
	.byte	0xa
	.byte	0x9a
	.byte	0xd
	.long	0x296
	.uleb128 0x2
	.long	.LASF90
	.byte	0x8
	.byte	0xa
	.byte	0x9b
	.byte	0x8
	.long	0x58a
	.uleb128 0x8
	.long	.LASF91
	.byte	0xa
	.byte	0x9c
	.byte	0x8
	.long	0x58a
	.byte	0
	.byte	0
	.uleb128 0x11
	.long	0x296
	.long	0x59a
	.uleb128 0x12
	.long	0x82
	.byte	0x1
	.byte	0
	.uleb128 0x9
	.long	.LASF92
	.byte	0xa
	.byte	0x9b
	.byte	0x30
	.long	0x56f
	.uleb128 0x9
	.long	.LASF93
	.byte	0xa
	.byte	0x9c
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF94
	.byte	0xa
	.byte	0x9d
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF95
	.byte	0xa
	.byte	0x9e
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF96
	.byte	0xa
	.byte	0x9f
	.byte	0x16
	.long	0x209
	.uleb128 0x9
	.long	.LASF97
	.byte	0xa
	.byte	0xa0
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF98
	.byte	0xa
	.byte	0xa1
	.byte	0x16
	.long	0x209
	.uleb128 0x9
	.long	.LASF99
	.byte	0xa
	.byte	0xa2
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF100
	.byte	0xa
	.byte	0xa3
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF101
	.byte	0xa
	.byte	0xa5
	.byte	0xd
	.long	0x296
	.uleb128 0x9
	.long	.LASF102
	.byte	0xa
	.byte	0xa6
	.byte	0xd
	.long	0x296
	.uleb128 0x9
	.long	.LASF103
	.byte	0xa
	.byte	0xa9
	.byte	0xd
	.long	0x296
	.uleb128 0x9
	.long	.LASF104
	.byte	0xa
	.byte	0xac
	.byte	0xf
	.long	0x180
	.uleb128 0x9
	.long	.LASF105
	.byte	0xa
	.byte	0xaf
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF106
	.byte	0xa
	.byte	0xb4
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF107
	.byte	0xa
	.byte	0xb5
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF108
	.byte	0xa
	.byte	0xb8
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF109
	.byte	0xa
	.byte	0xb9
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF110
	.byte	0xa
	.byte	0xbc
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF111
	.byte	0xa
	.byte	0xbd
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF112
	.byte	0xa
	.byte	0xc0
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF113
	.byte	0xa
	.byte	0xc2
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF114
	.byte	0xa
	.byte	0xc5
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF115
	.byte	0xa
	.byte	0xc7
	.byte	0x17
	.long	0x82
	.uleb128 0x9
	.long	.LASF116
	.byte	0xa
	.byte	0xcb
	.byte	0x13
	.long	0x557
	.uleb128 0x9
	.long	.LASF117
	.byte	0xa
	.byte	0xcc
	.byte	0xf
	.long	0x6d2
	.uleb128 0xd
	.byte	0x8
	.long	0x6d8
	.uleb128 0x4
	.byte	0x1
	.byte	0x6
	.long	.LASF118
	.uleb128 0x5
	.long	0x6d8
	.uleb128 0x9
	.long	.LASF119
	.byte	0xa
	.byte	0xcf
	.byte	0xe
	.long	0x2f9
	.uleb128 0x9
	.long	.LASF120
	.byte	0xa
	.byte	0xd2
	.byte	0x16
	.long	0x209
	.uleb128 0x9
	.long	.LASF121
	.byte	0xa
	.byte	0xd7
	.byte	0xd
	.long	0x296
	.uleb128 0x6
	.long	.LASF123
	.byte	0x4
	.byte	0xb
	.byte	0xd
	.byte	0x7
	.long	0x72e
	.uleb128 0x7
	.long	.LASF124
	.byte	0xb
	.byte	0xe
	.byte	0x11
	.long	0x209
	.uleb128 0x7
	.long	.LASF125
	.byte	0xb
	.byte	0xf
	.byte	0x9
	.long	0x72e
	.byte	0
	.uleb128 0x11
	.long	0x6d8
	.long	0x73e
	.uleb128 0x12
	.long	0x82
	.byte	0x3
	.byte	0
	.uleb128 0x2
	.long	.LASF126
	.byte	0x8
	.byte	0xb
	.byte	0xd
	.byte	0x8
	.long	0x766
	.uleb128 0x8
	.long	.LASF127
	.byte	0xb
	.byte	0xe
	.byte	0x8
	.long	0x296
	.byte	0
	.uleb128 0x8
	.long	.LASF128
	.byte	0xb
	.byte	0xf
	.byte	0x26
	.long	0x708
	.byte	0x4
	.byte	0
	.uleb128 0x9
	.long	.LASF129
	.byte	0xb
	.byte	0xd
	.byte	0x31
	.long	0x73e
	.uleb128 0x2
	.long	.LASF130
	.byte	0x10
	.byte	0xc
	.byte	0xa
	.byte	0x8
	.long	0x79a
	.uleb128 0x8
	.long	.LASF131
	.byte	0xc
	.byte	0xb
	.byte	0xc
	.long	0x54b
	.byte	0
	.uleb128 0x8
	.long	.LASF132
	.byte	0xc
	.byte	0xc
	.byte	0x10
	.long	0x766
	.byte	0x8
	.byte	0
	.uleb128 0x9
	.long	.LASF133
	.byte	0xc
	.byte	0xa
	.byte	0x1a
	.long	0x772
	.uleb128 0x2
	.long	.LASF134
	.byte	0x10
	.byte	0xd
	.byte	0xb
	.byte	0x8
	.long	0x7ce
	.uleb128 0x8
	.long	.LASF131
	.byte	0xd
	.byte	0xc
	.byte	0xe
	.long	0x557
	.byte	0
	.uleb128 0x8
	.long	.LASF132
	.byte	0xd
	.byte	0xd
	.byte	0x10
	.long	0x766
	.byte	0x8
	.byte	0
	.uleb128 0x9
	.long	.LASF135
	.byte	0xd
	.byte	0xb
	.byte	0x1c
	.long	0x7a6
	.uleb128 0x9
	.long	.LASF136
	.byte	0xe
	.byte	0x6
	.byte	0x19
	.long	0x7e6
	.uleb128 0x2
	.long	.LASF137
	.byte	0xd8
	.byte	0xf
	.byte	0x33
	.byte	0x8
	.long	0x997
	.uleb128 0x8
	.long	.LASF138
	.byte	0xf
	.byte	0x34
	.byte	0x8
	.long	0x296
	.byte	0
	.uleb128 0x8
	.long	.LASF139
	.byte	0xf
	.byte	0x35
	.byte	0xa
	.long	0x6d2
	.byte	0x8
	.uleb128 0x8
	.long	.LASF140
	.byte	0xf
	.byte	0x36
	.byte	0xa
	.long	0x6d2
	.byte	0x10
	.uleb128 0x8
	.long	.LASF141
	.byte	0xf
	.byte	0x37
	.byte	0xa
	.long	0x6d2
	.byte	0x18
	.uleb128 0x8
	.long	.LASF142
	.byte	0xf
	.byte	0x38
	.byte	0xa
	.long	0x6d2
	.byte	0x20
	.uleb128 0x8
	.long	.LASF143
	.byte	0xf
	.byte	0x39
	.byte	0xa
	.long	0x6d2
	.byte	0x28
	.uleb128 0x8
	.long	.LASF144
	.byte	0xf
	.byte	0x3a
	.byte	0xa
	.long	0x6d2
	.byte	0x30
	.uleb128 0x8
	.long	.LASF145
	.byte	0xf
	.byte	0x3b
	.byte	0xa
	.long	0x6d2
	.byte	0x38
	.uleb128 0x8
	.long	.LASF146
	.byte	0xf
	.byte	0x3c
	.byte	0xa
	.long	0x6d2
	.byte	0x40
	.uleb128 0x8
	.long	.LASF147
	.byte	0xf
	.byte	0x3d
	.byte	0xa
	.long	0x6d2
	.byte	0x48
	.uleb128 0x8
	.long	.LASF148
	.byte	0xf
	.byte	0x3e
	.byte	0xa
	.long	0x6d2
	.byte	0x50
	.uleb128 0x8
	.long	.LASF149
	.byte	0xf
	.byte	0x3f
	.byte	0xa
	.long	0x6d2
	.byte	0x58
	.uleb128 0x8
	.long	.LASF150
	.byte	0xf
	.byte	0x40
	.byte	0x17
	.long	0x9b0
	.byte	0x60
	.uleb128 0x8
	.long	.LASF151
	.byte	0xf
	.byte	0x41
	.byte	0x15
	.long	0x9b6
	.byte	0x68
	.uleb128 0x8
	.long	.LASF152
	.byte	0xf
	.byte	0x42
	.byte	0x8
	.long	0x296
	.byte	0x70
	.uleb128 0x3
	.long	.LASF153
	.byte	0xf
	.byte	0x43
	.byte	0x8
	.long	0x296
	.byte	0x4
	.byte	0x18
	.byte	0x8
	.byte	0x74
	.uleb128 0x8
	.long	.LASF154
	.byte	0xf
	.byte	0x44
	.byte	0x9
	.long	0x9bc
	.byte	0x77
	.uleb128 0x8
	.long	.LASF155
	.byte	0xf
	.byte	0x45
	.byte	0xc
	.long	0x54b
	.byte	0x78
	.uleb128 0x8
	.long	.LASF156
	.byte	0xf
	.byte	0x46
	.byte	0x13
	.long	0x89
	.byte	0x80
	.uleb128 0x8
	.long	.LASF157
	.byte	0xf
	.byte	0x47
	.byte	0x10
	.long	0x40c
	.byte	0x82
	.uleb128 0x8
	.long	.LASF158
	.byte	0xf
	.byte	0x48
	.byte	0x9
	.long	0x9bc
	.byte	0x83
	.uleb128 0x8
	.long	.LASF159
	.byte	0xf
	.byte	0x49
	.byte	0x10
	.long	0x9cc
	.byte	0x88
	.uleb128 0x8
	.long	.LASF160
	.byte	0xf
	.byte	0x4a
	.byte	0xe
	.long	0x557
	.byte	0x90
	.uleb128 0x8
	.long	.LASF161
	.byte	0xf
	.byte	0x4b
	.byte	0x18
	.long	0x9d7
	.byte	0x98
	.uleb128 0x8
	.long	.LASF162
	.byte	0xf
	.byte	0x4c
	.byte	0x1a
	.long	0x9e2
	.byte	0xa0
	.uleb128 0x8
	.long	.LASF163
	.byte	0xf
	.byte	0x4d
	.byte	0x15
	.long	0x9b6
	.byte	0xa8
	.uleb128 0x8
	.long	.LASF164
	.byte	0xf
	.byte	0x4e
	.byte	0xa
	.long	0x180
	.byte	0xb0
	.uleb128 0x8
	.long	.LASF165
	.byte	0xf
	.byte	0x4f
	.byte	0x16
	.long	0x9e8
	.byte	0xb8
	.uleb128 0x8
	.long	.LASF166
	.byte	0xf
	.byte	0x50
	.byte	0x8
	.long	0x296
	.byte	0xc0
	.uleb128 0x8
	.long	.LASF167
	.byte	0xf
	.byte	0x51
	.byte	0x8
	.long	0x296
	.byte	0xc4
	.uleb128 0x8
	.long	.LASF168
	.byte	0xf
	.byte	0x52
	.byte	0xf
	.long	0x45b
	.byte	0xc8
	.uleb128 0x8
	.long	.LASF169
	.byte	0xf
	.byte	0x53
	.byte	0x9
	.long	0x9ee
	.byte	0xd0
	.byte	0
	.uleb128 0x9
	.long	.LASF170
	.byte	0x10
	.byte	0x8
	.byte	0x19
	.long	0x7e6
	.uleb128 0x19
	.long	.LASF190
	.byte	0xf
	.byte	0x2d
	.byte	0xe
	.uleb128 0xf
	.long	.LASF171
	.uleb128 0xd
	.byte	0x8
	.long	0x9ab
	.uleb128 0xd
	.byte	0x8
	.long	0x7e6
	.uleb128 0x11
	.long	0x6d8
	.long	0x9cc
	.uleb128 0x12
	.long	0x82
	.byte	0
	.byte	0
	.uleb128 0xd
	.byte	0x8
	.long	0x9a3
	.uleb128 0xf
	.long	.LASF172
	.uleb128 0xd
	.byte	0x8
	.long	0x9d2
	.uleb128 0xf
	.long	.LASF173
	.uleb128 0xd
	.byte	0x8
	.long	0x9dd
	.uleb128 0xd
	.byte	0x8
	.long	0x9b6
	.uleb128 0x11
	.long	0x6d8
	.long	0x9fe
	.uleb128 0x12
	.long	0x82
	.byte	0x7
	.byte	0
	.uleb128 0x9
	.long	.LASF174
	.byte	0x11
	.byte	0x59
	.byte	0x12
	.long	0x79a
	.uleb128 0x1a
	.long	.LASF175
	.byte	0x5
	.byte	0x54
	.byte	0x19
	.long	0x180
	.uleb128 0x1a
	.long	.LASF176
	.byte	0x5
	.byte	0xb0
	.byte	0xe
	.long	0xa22
	.uleb128 0x4
	.byte	0x1
	.byte	0x2
	.long	.LASF177
	.uleb128 0x1a
	.long	.LASF178
	.byte	0x5
	.byte	0xd7
	.byte	0x2a
	.long	0x210
	.uleb128 0x1a
	.long	.LASF179
	.byte	0x11
	.byte	0x99
	.byte	0xe
	.long	0xa41
	.uleb128 0xd
	.byte	0x8
	.long	0x997
	.uleb128 0x1a
	.long	.LASF180
	.byte	0x11
	.byte	0x9a
	.byte	0xe
	.long	0xa41
	.uleb128 0x1a
	.long	.LASF181
	.byte	0x11
	.byte	0x9b
	.byte	0xe
	.long	0xa41
	.uleb128 0x1b
	.long	.LASF182
	.byte	0x12
	.byte	0x35
	.byte	0xc
	.long	0x296
	.long	0xa7b
	.uleb128 0xb
	.long	0x296
	.uleb128 0xb
	.long	0xa81
	.uleb128 0x1c
	.byte	0
	.uleb128 0xd
	.byte	0x8
	.long	0x6df
	.uleb128 0x1d
	.long	0xa7b
	.uleb128 0x1e
	.long	.LASF191
	.byte	0x7
	.value	0x2b3
	.byte	0x47
	.long	0xa99
	.uleb128 0xb
	.long	0x180
	.byte	0
	.uleb128 0x1f
	.long	.LASF183
	.byte	0x7
	.value	0x2a5
	.byte	0x1f
	.long	0x180
	.long	0xab0
	.uleb128 0xb
	.long	0x82
	.byte	0
	.uleb128 0x20
	.long	.LASF192
	.byte	0x2
	.byte	0x8
	.byte	0x5
	.long	0x296
	.quad	.LFB42
	.quad	.LFE42-.LFB42
	.uleb128 0x1
	.byte	0x9c
	.long	0xb14
	.uleb128 0x21
	.string	"m"
	.byte	0x2
	.byte	0xa
	.byte	0x9
	.long	0x180
	.long	.LLST0
	.uleb128 0x21
	.string	"tmp"
	.byte	0x2
	.byte	0xb
	.byte	0x9
	.long	0x180
	.long	.LLST0
	.uleb128 0x22
	.long	0xb14
	.quad	.LBB4
	.long	.Ldebug_ranges0+0
	.byte	0x2
	.byte	0xb
	.byte	0x3
	.uleb128 0x23
	.long	0xb21
	.long	.LLST2
	.uleb128 0x24
	.long	0xb2e
	.byte	0
	.byte	0
	.uleb128 0x25
	.long	.LASF184
	.byte	0x3
	.byte	0x73
	.byte	0x5a
	.long	0x296
	.byte	0x3
	.uleb128 0x26
	.long	.LASF193
	.byte	0x3
	.byte	0x73
	.byte	0x7d
	.long	0xa81
	.uleb128 0x1c
	.uleb128 0x27
	.string	"tmp"
	.byte	0x3
	.byte	0x76
	.byte	0x7
	.long	0x296
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.uleb128 0x43
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xd
	.uleb128 0xb
	.uleb128 0xc
	.uleb128 0xb
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x17
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x37
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x57
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x24
	.uleb128 0x34
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x25
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x34
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x26
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x27
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.quad	.LVL1
	.quad	.LVL2
	.value	0x1
	.byte	0x50
	.quad	.LVL2
	.quad	.LVL3-1
	.value	0x1
	.byte	0x51
	.quad	.LVL3-1
	.quad	.LVL5
	.value	0x1
	.byte	0x53
	.quad	0
	.quad	0
.LLST2:
	.quad	.LVL1
	.quad	.LVL3
	.value	0xa
	.byte	0x3
	.quad	.LC0
	.byte	0x9f
	.quad	0
	.quad	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.LFB42
	.quad	.LFE42-.LFB42
	.quad	0
	.quad	0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.quad	.LBB4
	.quad	.LBE4
	.quad	.LBB7
	.quad	.LBE7
	.quad	0
	.quad	0
	.quad	.LFB42
	.quad	.LFE42
	.quad	0
	.quad	0
	.section	.debug_macinfo,"",@progbits
.Ldebug_macinfo0:
	.byte	0x3
	.uleb128 0
	.uleb128 0x1
	.byte	0x4
	.byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF87:
	.string	"__off_t"
.LASF82:
	.string	"__gid_t"
.LASF139:
	.string	"_IO_read_ptr"
.LASF183:
	.string	"malloc"
.LASF151:
	.string	"_chain"
.LASF22:
	.string	"__annonCompField1"
.LASF41:
	.string	"quot"
.LASF178:
	.string	"__liballocs_ool_cache"
.LASF77:
	.string	"__u_quad_t"
.LASF38:
	.string	"size_t"
.LASF1:
	.string	"alloc_site"
.LASF158:
	.string	"_shortbuf"
.LASF109:
	.string	"__fsblkcnt64_t"
.LASF61:
	.string	"__uint8_t"
.LASF51:
	.string	"gp_offset"
.LASF43:
	.string	"__anonstruct_ldiv_t_112316392"
.LASF14:
	.string	"_ignore"
.LASF104:
	.string	"__timer_t"
.LASF26:
	.string	"period"
.LASF8:
	.string	"insert_with_type"
.LASF29:
	.string	"next_mru"
.LASF66:
	.string	"__int64_t"
.LASF129:
	.string	"__mbstate_t"
.LASF101:
	.string	"__daddr_t"
.LASF73:
	.string	"__uint_least32_t"
.LASF59:
	.string	"__int8_t"
.LASF74:
	.string	"__int_least64_t"
.LASF161:
	.string	"_codecvt"
.LASF28:
	.string	"prev_mru"
.LASF42:
	.string	"div_t"
.LASF47:
	.string	"long long int"
.LASF60:
	.string	"signed char"
.LASF35:
	.string	"head_mru"
.LASF85:
	.string	"__mode_t"
.LASF102:
	.string	"__key_t"
.LASF184:
	.string	"printf"
.LASF152:
	.string	"_fileno"
.LASF140:
	.string	"_IO_read_end"
.LASF106:
	.string	"__blkcnt_t"
.LASF44:
	.string	"long int"
.LASF138:
	.string	"_flags"
.LASF188:
	.string	"__builtin_va_list"
.LASF113:
	.string	"__ssize_t"
.LASF136:
	.string	"__FILE"
.LASF146:
	.string	"_IO_buf_end"
.LASF156:
	.string	"_cur_column"
.LASF11:
	.string	"uniqtype_shifted"
.LASF172:
	.string	"_IO_codecvt"
.LASF76:
	.string	"__quad_t"
.LASF182:
	.string	"__printf_chk"
.LASF176:
	.string	"__liballocs_is_initialized"
.LASF155:
	.string	"_old_offset"
.LASF160:
	.string	"_offset"
.LASF186:
	.string	"hello-via-wrapper.i"
.LASF130:
	.string	"_G_fpos_t"
.LASF65:
	.string	"__uint32_t"
.LASF131:
	.string	"__pos"
.LASF23:
	.string	"obj_base"
.LASF31:
	.string	"__liballocs_memrange_cache"
.LASF32:
	.string	"validity"
.LASF100:
	.string	"__suseconds64_t"
.LASF80:
	.string	"__dev_t"
.LASF16:
	.string	"with_type"
.LASF70:
	.string	"__int_least16_t"
.LASF126:
	.string	"__anonstruct___mbstate_t_4899493"
.LASF174:
	.string	"fpos_t"
.LASF18:
	.string	"insert"
.LASF30:
	.string	"unsigned int"
.LASF164:
	.string	"_freeres_buf"
.LASF91:
	.string	"__val"
.LASF53:
	.string	"overflow_arg_area"
.LASF90:
	.string	"__anonstruct___fsid_t_288746708"
.LASF5:
	.string	"long unsigned int"
.LASF57:
	.string	"__u_int"
.LASF99:
	.string	"__suseconds_t"
.LASF143:
	.string	"_IO_write_ptr"
.LASF36:
	.string	"tail_mru"
.LASF33:
	.string	"size_plus_one"
.LASF69:
	.string	"__uint_least8_t"
.LASF79:
	.string	"__uintmax_t"
.LASF78:
	.string	"__intmax_t"
.LASF147:
	.string	"_IO_save_base"
.LASF124:
	.string	"__wch"
.LASF17:
	.string	"common"
.LASF95:
	.string	"__rlim64_t"
.LASF10:
	.string	"alloc_site_id"
.LASF25:
	.string	"uniqtype"
.LASF93:
	.string	"__clock_t"
.LASF24:
	.string	"obj_limit"
.LASF159:
	.string	"_lock"
.LASF153:
	.string	"_flags2"
.LASF166:
	.string	"_mode"
.LASF180:
	.string	"stdout"
.LASF40:
	.string	"__anonstruct_div_t_946224742"
.LASF45:
	.string	"ldiv_t"
.LASF72:
	.string	"__int_least32_t"
.LASF119:
	.string	"__intptr_t"
.LASF94:
	.string	"__rlim_t"
.LASF2:
	.string	"unused"
.LASF37:
	.string	"entries"
.LASF55:
	.string	"__u_char"
.LASF179:
	.string	"stdin"
.LASF114:
	.string	"__syscall_slong_t"
.LASF50:
	.string	"__gnuc_va_list"
.LASF191:
	.string	"free"
.LASF19:
	.string	"sizefn_t"
.LASF144:
	.string	"_IO_write_end"
.LASF165:
	.string	"_prevchain"
.LASF96:
	.string	"__id_t"
.LASF88:
	.string	"__off64_t"
.LASF115:
	.string	"__syscall_ulong_t"
.LASF112:
	.string	"__fsword_t"
.LASF190:
	.string	"_IO_lock_t"
.LASF137:
	.string	"_IO_FILE"
.LASF105:
	.string	"__blksize_t"
.LASF67:
	.string	"__uint64_t"
.LASF133:
	.string	"__fpos_t"
.LASF117:
	.string	"__caddr_t"
.LASF171:
	.string	"_IO_marker"
.LASF150:
	.string	"_markers"
.LASF46:
	.string	"__anonstruct_lldiv_t_1059493607"
.LASF103:
	.string	"__clockid_t"
.LASF107:
	.string	"__blkcnt64_t"
.LASF75:
	.string	"__uint_least64_t"
.LASF116:
	.string	"__loff_t"
.LASF177:
	.string	"_Bool"
.LASF4:
	.string	"unsigned char"
.LASF48:
	.string	"lldiv_t"
.LASF125:
	.string	"__wchb"
.LASF108:
	.string	"__fsblkcnt_t"
.LASF89:
	.string	"__pid_t"
.LASF12:
	.string	"short int"
.LASF173:
	.string	"_IO_wide_data"
.LASF167:
	.string	"_unused3"
.LASF157:
	.string	"_vtable_offset"
.LASF97:
	.string	"__time_t"
.LASF170:
	.string	"FILE"
.LASF54:
	.string	"reg_save_area"
.LASF83:
	.string	"__ino_t"
.LASF127:
	.string	"__count"
.LASF128:
	.string	"__value"
.LASF27:
	.string	"depth"
.LASF118:
	.string	"char"
.LASF120:
	.string	"__socklen_t"
.LASF63:
	.string	"__uint16_t"
.LASF187:
	.string	"/home/zoltan/Develop/stackscan/contrib/liballocs/tests/hello-via-wrapper"
.LASF111:
	.string	"__fsfilcnt64_t"
.LASF64:
	.string	"__int32_t"
.LASF7:
	.string	"insert_initial"
.LASF81:
	.string	"__uid_t"
.LASF110:
	.string	"__fsfilcnt_t"
.LASF154:
	.string	"_short_backupbuf"
.LASF122:
	.string	"__anonunion____missing_field_name_1007889555"
.LASF141:
	.string	"_IO_read_base"
.LASF149:
	.string	"_IO_save_end"
.LASF71:
	.string	"__uint_least16_t"
.LASF6:
	.string	"short unsigned int"
.LASF21:
	.string	"__liballocs_memrange_cache_entry_s"
.LASF193:
	.string	"__fmt"
.LASF39:
	.string	"wchar_t"
.LASF92:
	.string	"__fsid_t"
.LASF134:
	.string	"_G_fpos64_t"
.LASF98:
	.string	"__useconds_t"
.LASF169:
	.string	"_unused2"
.LASF181:
	.string	"stderr"
.LASF13:
	.string	"insert_common"
.LASF3:
	.string	"lifetime_policies"
.LASF175:
	.string	"__current_allocsite"
.LASF15:
	.string	"initial"
.LASF68:
	.string	"__int_least8_t"
.LASF0:
	.string	"always_0"
.LASF9:
	.string	"always_1"
.LASF148:
	.string	"_IO_backup_base"
.LASF52:
	.string	"fp_offset"
.LASF135:
	.string	"__fpos64_t"
.LASF62:
	.string	"__int16_t"
.LASF49:
	.string	"__compar_fn_t"
.LASF163:
	.string	"_freeres_list"
.LASF58:
	.string	"__u_long"
.LASF162:
	.string	"_wide_data"
.LASF56:
	.string	"__u_short"
.LASF84:
	.string	"__ino64_t"
.LASF132:
	.string	"__state"
.LASF123:
	.string	"__anonunion___value_4899494"
.LASF192:
	.string	"main"
.LASF142:
	.string	"_IO_write_base"
.LASF121:
	.string	"__sig_atomic_t"
.LASF86:
	.string	"__nlink_t"
.LASF145:
	.string	"_IO_buf_base"
.LASF168:
	.string	"_total_written"
.LASF189:
	.string	"__va_list_tag"
.LASF185:
	.ascii	"GNU C99 15.2.0 -mtune=generic -march=x86-64 -g3 -gdwarf-4 -g"
	.ascii	"3 -gdwarf-4 -g3 -gdwarf-4 -gstrict-dwarf"
	.string	" -g3 -ggdb -gdwarf-4 -O2 -O2 -O2 -std=c99 -std=c99 -std=c99 -fno-lto -fstack-protector-strong -fcf-protection=full -fzero-init-padding-bits=all -fno-eliminate-unused-debug-types -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection"
.LASF20:
	.string	"lifetime_insert_t"
.LASF34:
	.string	"next_victim"
	.ident	"GCC: (Ubuntu 15.2.0-16ubuntu1) 15.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
