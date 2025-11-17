	.file	"alloca-clang.i"
                                        # Start of file scope inline assembly
.pushsection .allocs_srcallocs,"a",@progbits
	.ascii "/home/zoltan/Develop/liballocs/tests/alloca-clang/alloca-clang.c\t41\t41\t__builtin_alloca\t__uniqtype__int\t1\n"
	.popsection

                                        # End of file scope inline assembly
	.file	1 "/home/zoltan/Develop/liballocs/tests/alloca-clang/alloca-clang.c"
	.file	2 "/home/zoltan/Develop/liballocs/tests/../include/uniqtype-defs.h"
	.file	3 "/home/zoltan/Develop/liballocs/include/malloc-meta.h"
	.file	4 "/home/zoltan/Develop/liballocs/include/liballocs_cil_inlines.h"
	.file	5 "/usr/lib/llvm-20/lib/clang/20/include/__stddef_ptrdiff_t.h"
	.file	6 "/usr/lib/llvm-20/lib/clang/20/include/__stddef_size_t.h"
	.file	7 "/usr/lib/llvm-20/lib/clang/20/include/__stddef_wchar_t.h"
	.text
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
.Lfunc_begin0:
	.loc	1 40 0                          # alloca-clang.c:40:0
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
.Ltmp0:
	pushq	%r14
	pushq	%rbx
	subq	$224, %rsp
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
.Ltmp1:
	#DEBUG_VALUE: main:__liballocs_alloca_cleanup_local <- [DW_OP_constu 24, DW_OP_minus, DW_OP_deref] $rbp
	#DEBUG_VALUE: main:__liballocs_alloca_cleanup_local <- 0
	#DEBUG_VALUE: main:__cil_tmp9 <- 208
	.loc	1 41 3 prologue_end             # alloca-clang.c:41:3
	#APP
	   call .L__monalloca_alloca_label_alloca$2dclang$2ec_41_1_0
.L__monalloca_alloca_label_alloca$2dclang$2ec_41_1_0: 
pop %rcx

	#NO_APP
.Ltmp2:
	#DEBUG_VALUE: main:__cil_tmp8 <- $rcx
	#DEBUG_VALUE: __liballocs_notify_and_adjust_alloca:caller <- $rcx
	#DEBUG_VALUE: main:__cil_tmp10 <- [DW_OP_constu 240, DW_OP_minus, DW_OP_stack_value] $rbp
	#DEBUG_VALUE: __liballocs_notify_and_adjust_alloca:allocated <- [DW_OP_constu 240, DW_OP_minus, DW_OP_stack_value] $rbp
	#DEBUG_VALUE: __liballocs_notify_and_adjust_alloca:orig_size <- 168
	#DEBUG_VALUE: __liballocs_notify_and_adjust_alloca:tweaked_size <- 208
	#DEBUG_VALUE: __liballocs_notify_and_adjust_alloca:frame_counter <- [DW_OP_constu 24, DW_OP_minus, DW_OP_stack_value] $rbp
	.loc	4 145 3                         # include/liballocs_cil_inlines.h:145:3
	xorps	%xmm0, %xmm0
	movaps	%xmm0, -240(%rbp)
	movaps	%xmm0, -224(%rbp)
	movaps	%xmm0, -208(%rbp)
	movaps	%xmm0, -192(%rbp)
	movaps	%xmm0, -176(%rbp)
	movaps	%xmm0, -160(%rbp)
	movaps	%xmm0, -144(%rbp)
	movaps	%xmm0, -128(%rbp)
	movaps	%xmm0, -112(%rbp)
	movaps	%xmm0, -96(%rbp)
	movaps	%xmm0, -80(%rbp)
	movaps	%xmm0, -64(%rbp)
	movaps	%xmm0, -48(%rbp)
	leaq	-224(%rbp), %rbx
.Ltmp3:
	#DEBUG_VALUE: __liballocs_notify_and_adjust_alloca:header_effective_size <- [DW_OP_LLVM_arg 0, DW_OP_LLVM_arg 1, DW_OP_constu 240, DW_OP_minus, DW_OP_minus, DW_OP_stack_value] $rbx, $rbp
	#DEBUG_VALUE: __liballocs_notify_and_adjust_alloca:userchunk <- $rbx
	#DEBUG_VALUE: main:tmp <- $rbx
	#DEBUG_VALUE: main:a <- $rbx
	#DEBUG_VALUE: __liballocs_notify_and_adjust_alloca:non_header_size <- 192
	.loc	4 155 37                        # include/liballocs_cil_inlines.h:155:37
	movq	$192, -232(%rbp)
	.loc	4 165 18                        # include/liballocs_cil_inlines.h:165:18
	movq	$192, -24(%rbp)
.Ltmp4:
	#DEBUG_VALUE: main:__liballocs_alloca_cleanup_local <- [DW_OP_constu 24, DW_OP_minus, DW_OP_deref] $rbp
	#DEBUG_VALUE: __liballocs_notify_and_adjust_alloca:tmp <- $rbp
	.loc	4 113 3                         # include/liballocs_cil_inlines.h:113:3
	#APP
	movq %rsp, %r8

	#NO_APP
.Ltmp5:
	#DEBUG_VALUE: __liballocs_get_sp:our_sp <- $r8
	#DEBUG_VALUE: __liballocs_notify_and_adjust_alloca:tmp___0 <- $r8
	.loc	4 0 3 is_stmt 0                 # include/liballocs_cil_inlines.h:0:3
	leaq	-24(%rbp), %rdx
.Ltmp6:
	.loc	4 168 3 is_stmt 1               # include/liballocs_cil_inlines.h:168:3
	movl	$168, %esi
	movq	%rbx, %rdi
	movq	%rbp, %r9
	callq	__alloca_allocator_notify@PLT
.Ltmp7:
	.loc	1 42 13                         # alloca-clang.c:42:13
	movq	%rbx, %rdi
	callq	__liballocs_get_alloc_type@PLT
.Ltmp8:
	movq	%rax, %r14
.Ltmp9:
	#DEBUG_VALUE: main:tmp___0 <- $r14
	#DEBUG_VALUE: main:got_type <- $r14
	.loc	1 43 13                         # alloca-clang.c:43:13
	leaq	.L.str(%rip), %rsi
	movq	$-1, %rdi
	callq	dlsym@PLT
.Ltmp10:
	movq	%rax, %rbx
.Ltmp11:
	#DEBUG_LABEL: while_continue
	.loc	1 44 11                         # alloca-clang.c:44:11
	testq	%rax, %rax
	.loc	1 44 9 is_stmt 0                # alloca-clang.c:44:9
	je	.LBB0_1
.Ltmp12:
# %bb.2:
	#DEBUG_VALUE: main:__liballocs_alloca_cleanup_local <- [DW_OP_constu 24, DW_OP_minus, DW_OP_deref] $rbp
	#DEBUG_VALUE: main:__cil_tmp9 <- 208
	#DEBUG_VALUE: main:__cil_tmp10 <- [DW_OP_constu 240, DW_OP_minus, DW_OP_stack_value] $rbp
	#DEBUG_VALUE: main:tmp___0 <- $r14
	#DEBUG_VALUE: main:got_type <- $r14
	#DEBUG_LABEL: while_break
	#DEBUG_LABEL: while_continue___0
	.loc	1 45 11 is_stmt 1               # alloca-clang.c:45:11
	testq	%r14, %r14
	.loc	1 45 9 is_stmt 0                # alloca-clang.c:45:9
	je	.LBB0_3
.Ltmp13:
.LBB0_4:
	#DEBUG_VALUE: main:__liballocs_alloca_cleanup_local <- [DW_OP_constu 24, DW_OP_minus, DW_OP_deref] $rbp
	#DEBUG_VALUE: main:__cil_tmp9 <- 208
	#DEBUG_VALUE: main:__cil_tmp10 <- [DW_OP_constu 240, DW_OP_minus, DW_OP_stack_value] $rbp
	#DEBUG_VALUE: main:tmp___0 <- $r14
	#DEBUG_VALUE: main:got_type <- $r14
	#DEBUG_LABEL: while_break___0
	#DEBUG_LABEL: while_continue___1
	.loc	1 46 9 is_stmt 1                # alloca-clang.c:46:9
	testb	$1, 12(%r14)
	je	.LBB0_5
	jmp	.LBB0_7
.Ltmp14:
.LBB0_1:
	#DEBUG_VALUE: main:__liballocs_alloca_cleanup_local <- [DW_OP_constu 24, DW_OP_minus, DW_OP_deref] $rbp
	#DEBUG_VALUE: main:__cil_tmp9 <- 208
	#DEBUG_VALUE: main:__cil_tmp10 <- [DW_OP_constu 240, DW_OP_minus, DW_OP_stack_value] $rbp
	#DEBUG_VALUE: main:tmp___0 <- $r14
	#DEBUG_VALUE: main:got_type <- $r14
	.loc	1 44 7                          # alloca-clang.c:44:7
	leaq	.L.str.1(%rip), %rdi
	leaq	.L.str.2(%rip), %rsi
	leaq	.L.str.3(%rip), %rcx
	movl	$38, %edx
	callq	__assert_fail@PLT
.Ltmp15:
	#DEBUG_LABEL: while_break
	#DEBUG_LABEL: while_continue___0
	.loc	1 45 11                         # alloca-clang.c:45:11
	testq	%r14, %r14
	.loc	1 45 9 is_stmt 0                # alloca-clang.c:45:9
	jne	.LBB0_4
.Ltmp16:
.LBB0_3:
	#DEBUG_VALUE: main:__liballocs_alloca_cleanup_local <- [DW_OP_constu 24, DW_OP_minus, DW_OP_deref] $rbp
	#DEBUG_VALUE: main:__cil_tmp9 <- 208
	#DEBUG_VALUE: main:__cil_tmp10 <- [DW_OP_constu 240, DW_OP_minus, DW_OP_stack_value] $rbp
	#DEBUG_VALUE: main:tmp___0 <- $r14
	#DEBUG_VALUE: main:got_type <- $r14
	.loc	1 45 7                          # alloca-clang.c:45:7
	leaq	.L.str.4(%rip), %rdi
	leaq	.L.str.2(%rip), %rsi
	leaq	.L.str.3(%rip), %rcx
	movl	$39, %edx
	callq	__assert_fail@PLT
.Ltmp17:
	#DEBUG_LABEL: while_break___0
	#DEBUG_LABEL: while_continue___1
	.loc	1 46 9 is_stmt 1                # alloca-clang.c:46:9
	testb	$1, 12(%r14)
	jne	.LBB0_7
.Ltmp18:
.LBB0_5:
	#DEBUG_VALUE: main:__liballocs_alloca_cleanup_local <- [DW_OP_constu 24, DW_OP_minus, DW_OP_deref] $rbp
	#DEBUG_VALUE: main:__cil_tmp9 <- 208
	#DEBUG_VALUE: main:__cil_tmp10 <- [DW_OP_constu 240, DW_OP_minus, DW_OP_stack_value] $rbp
	#DEBUG_VALUE: main:tmp___0 <- $r14
	#DEBUG_VALUE: main:got_type <- $r14
	.loc	1 46 7 is_stmt 0                # alloca-clang.c:46:7
	leaq	.L.str.5(%rip), %rdi
	leaq	.L.str.2(%rip), %rsi
	leaq	.L.str.3(%rip), %rcx
	movl	$40, %edx
	callq	__assert_fail@PLT
.Ltmp19:
	.loc	1 47 29 is_stmt 1               # alloca-clang.c:47:29
	testb	$1, 12(%r14)
.Ltmp20:
	#DEBUG_LABEL: while_break___1
	#DEBUG_LABEL: while_continue___2
	jne	.LBB0_7
.Ltmp21:
# %bb.6:
	#DEBUG_VALUE: main:__liballocs_alloca_cleanup_local <- [DW_OP_constu 24, DW_OP_minus, DW_OP_deref] $rbp
	#DEBUG_VALUE: main:__cil_tmp9 <- 208
	#DEBUG_VALUE: main:__cil_tmp10 <- [DW_OP_constu 240, DW_OP_minus, DW_OP_stack_value] $rbp
	#DEBUG_VALUE: main:tmp___0 <- $r14
	#DEBUG_VALUE: main:got_type <- $r14
	.loc	1 0 29 is_stmt 0                # alloca-clang.c:0:29
	xorl	%eax, %eax
	.loc	1 47 123                        # alloca-clang.c:47:123
	cmpq	%rbx, %rax
	.loc	1 47 9                          # alloca-clang.c:47:9
	jne	.LBB0_9
	jmp	.LBB0_10
.Ltmp22:
.LBB0_7:                                # %.thread
	#DEBUG_VALUE: main:__liballocs_alloca_cleanup_local <- [DW_OP_constu 24, DW_OP_minus, DW_OP_deref] $rbp
	#DEBUG_VALUE: main:__cil_tmp9 <- 208
	#DEBUG_VALUE: main:__cil_tmp10 <- [DW_OP_constu 240, DW_OP_minus, DW_OP_stack_value] $rbp
	#DEBUG_VALUE: main:tmp___0 <- $r14
	#DEBUG_VALUE: main:got_type <- $r14
	.loc	1 47 85 is_stmt 1               # alloca-clang.c:47:85
	movq	24(%r14), %rax
	.loc	1 47 123 is_stmt 0              # alloca-clang.c:47:123
	cmpq	%rbx, %rax
	.loc	1 47 9                          # alloca-clang.c:47:9
	je	.LBB0_10
.Ltmp23:
.LBB0_9:
	#DEBUG_VALUE: main:__liballocs_alloca_cleanup_local <- [DW_OP_constu 24, DW_OP_minus, DW_OP_deref] $rbp
	#DEBUG_VALUE: main:__cil_tmp9 <- 208
	#DEBUG_VALUE: main:__cil_tmp10 <- [DW_OP_constu 240, DW_OP_minus, DW_OP_stack_value] $rbp
	#DEBUG_VALUE: main:tmp___0 <- $r14
	#DEBUG_VALUE: main:got_type <- $r14
	.loc	1 47 7                          # alloca-clang.c:47:7
	leaq	.L.str.6(%rip), %rdi
	leaq	.L.str.2(%rip), %rsi
	leaq	.L.str.3(%rip), %rcx
	movl	$41, %edx
	callq	__assert_fail@PLT
.Ltmp24:
.LBB0_10:
	#DEBUG_VALUE: main:__liballocs_alloca_cleanup_local <- [DW_OP_constu 24, DW_OP_minus, DW_OP_deref] $rbp
	#DEBUG_VALUE: main:__cil_tmp9 <- 208
	#DEBUG_VALUE: main:__cil_tmp10 <- [DW_OP_constu 240, DW_OP_minus, DW_OP_stack_value] $rbp
	#DEBUG_VALUE: main:tmp___0 <- $r14
	#DEBUG_VALUE: main:got_type <- $r14
	#DEBUG_LABEL: while_break___2
	#DEBUG_VALUE: __liballocs_alloca_caller_frame_cleanup:counter <- [DW_OP_constu 24, DW_OP_minus, DW_OP_stack_value] $rbp
	#DEBUG_VALUE: __liballocs_alloca_caller_frame_cleanup:tmp <- $rbp
	.loc	1 0 7                           # alloca-clang.c:0:7
	leaq	-24(%rbp), %rdi
.Ltmp25:
	.loc	4 102 3 is_stmt 1               # include/liballocs_cil_inlines.h:102:3
	movq	%rbp, %rsi
	callq	__liballocs_unindex_stack_objects_counted_by@PLT
.Ltmp26:
	.loc	1 51 1                          # alloca-clang.c:51:1
	xorl	%eax, %eax
	.loc	1 51 1 epilogue_begin is_stmt 0 # alloca-clang.c:51:1
	addq	$224, %rsp
	popq	%rbx
	popq	%r14
.Ltmp27:
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Ltmp28:
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"__uniqtype__int$$32"
	.size	.L.str, 20

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"int_type"
	.size	.L.str.1, 9

	.type	.L.str.2,@object                # @.str.2
.L.str.2:
	.asciz	"alloca-clang.c"
	.size	.L.str.2, 15

	.type	.L.str.3,@object                # @.str.3
.L.str.3:
	.asciz	"main"
	.size	.L.str.3, 5

	.type	.L.str.4,@object                # @.str.4
.L.str.4:
	.asciz	"got_type"
	.size	.L.str.4, 9

	.type	.L.str.5,@object                # @.str.5
.L.str.5:
	.asciz	"UNIQTYPE_IS_ARRAY_TYPE(got_type)"
	.size	.L.str.5, 33

	.type	.L.str.6,@object                # @.str.6
.L.str.6:
	.asciz	"UNIQTYPE_ARRAY_ELEMENT_TYPE(got_type) == int_type"
	.size	.L.str.6, 50

	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
	.quad	.Ltmp1-.Lfunc_begin0
	.quad	.Ltmp4-.Lfunc_begin0
	.short	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.quad	.Ltmp4-.Lfunc_begin0
	.quad	.Lfunc_end0-.Lfunc_begin0
	.short	2                               # Loc expr size
	.byte	118                             # DW_OP_breg6
	.byte	104                             # -24
	.quad	0
	.quad	0
.Ldebug_loc1:
	.quad	.Ltmp2-.Lfunc_begin0
	.quad	.Ltmp7-.Lfunc_begin0
	.short	1                               # Loc expr size
	.byte	82                              # DW_OP_reg2
	.quad	0
	.quad	0
.Ldebug_loc2:
	.quad	.Ltmp2-.Lfunc_begin0
	.quad	.Lfunc_end0-.Lfunc_begin0
	.short	4                               # Loc expr size
	.byte	118                             # DW_OP_breg6
	.byte	144                             # -240
	.byte	126                             # 
	.byte	159                             # DW_OP_stack_value
	.quad	0
	.quad	0
.Ldebug_loc3:
	.quad	.Ltmp3-.Lfunc_begin0
	.quad	.Ltmp11-.Lfunc_begin0
	.short	7                               # Loc expr size
	.byte	115                             # DW_OP_breg3
	.byte	0                               # 0
	.byte	118                             # DW_OP_breg6
	.byte	144                             # -240
	.byte	126                             # 
	.byte	28                              # DW_OP_minus
	.byte	159                             # DW_OP_stack_value
	.quad	0
	.quad	0
.Ldebug_loc4:
	.quad	.Ltmp3-.Lfunc_begin0
	.quad	.Ltmp11-.Lfunc_begin0
	.short	1                               # Loc expr size
	.byte	83                              # DW_OP_reg3
	.quad	0
	.quad	0
.Ldebug_loc5:
	.quad	.Ltmp3-.Lfunc_begin0
	.quad	.Ltmp11-.Lfunc_begin0
	.short	1                               # Loc expr size
	.byte	83                              # DW_OP_reg3
	.quad	0
	.quad	0
.Ldebug_loc6:
	.quad	.Ltmp3-.Lfunc_begin0
	.quad	.Ltmp11-.Lfunc_begin0
	.short	1                               # Loc expr size
	.byte	83                              # DW_OP_reg3
	.quad	0
	.quad	0
.Ldebug_loc7:
	.quad	.Ltmp3-.Lfunc_begin0
	.quad	.Ltmp12-.Lfunc_begin0
	.short	4                               # Loc expr size
	.byte	16                              # DW_OP_constu
	.byte	192                             # 192
	.byte	1                               # 
	.byte	159                             # DW_OP_stack_value
	.quad	0
	.quad	0
.Ldebug_loc8:
	.quad	.Ltmp4-.Lfunc_begin0
	.quad	.Ltmp12-.Lfunc_begin0
	.short	1                               # Loc expr size
	.byte	86                              # DW_OP_reg6
	.quad	0
	.quad	0
.Ldebug_loc9:
	.quad	.Ltmp5-.Lfunc_begin0
	.quad	.Ltmp7-.Lfunc_begin0
	.short	1                               # Loc expr size
	.byte	88                              # DW_OP_reg8
	.quad	0
	.quad	0
.Ldebug_loc10:
	.quad	.Ltmp5-.Lfunc_begin0
	.quad	.Ltmp7-.Lfunc_begin0
	.short	1                               # Loc expr size
	.byte	88                              # DW_OP_reg8
	.quad	0
	.quad	0
.Ldebug_loc11:
	.quad	.Ltmp9-.Lfunc_begin0
	.quad	.Ltmp27-.Lfunc_begin0
	.short	1                               # Loc expr size
	.byte	94                              # DW_OP_reg14
	.quad	0
	.quad	0
.Ldebug_loc12:
	.quad	.Ltmp9-.Lfunc_begin0
	.quad	.Ltmp27-.Lfunc_begin0
	.short	1                               # Loc expr size
	.byte	94                              # DW_OP_reg14
	.quad	0
	.quad	0
	.section	.debug_abbrev,"",@progbits
	.byte	1                               # Abbreviation Code
	.byte	17                              # DW_TAG_compile_unit
	.byte	1                               # DW_CHILDREN_yes
	.byte	37                              # DW_AT_producer
	.byte	14                              # DW_FORM_strp
	.byte	19                              # DW_AT_language
	.byte	5                               # DW_FORM_data2
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	16                              # DW_AT_stmt_list
	.byte	23                              # DW_FORM_sec_offset
	.byte	27                              # DW_AT_comp_dir
	.byte	14                              # DW_FORM_strp
	.byte	17                              # DW_AT_low_pc
	.byte	1                               # DW_FORM_addr
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	2                               # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	2                               # DW_AT_location
	.byte	24                              # DW_FORM_exprloc
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	3                               # Abbreviation Code
	.byte	1                               # DW_TAG_array_type
	.byte	1                               # DW_CHILDREN_yes
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	4                               # Abbreviation Code
	.byte	33                              # DW_TAG_subrange_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	55                              # DW_AT_count
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	5                               # Abbreviation Code
	.byte	36                              # DW_TAG_base_type
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	62                              # DW_AT_encoding
	.byte	11                              # DW_FORM_data1
	.byte	11                              # DW_AT_byte_size
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	6                               # Abbreviation Code
	.byte	36                              # DW_TAG_base_type
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	11                              # DW_AT_byte_size
	.byte	11                              # DW_FORM_data1
	.byte	62                              # DW_AT_encoding
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	7                               # Abbreviation Code
	.byte	4                               # DW_TAG_enumeration_type
	.byte	1                               # DW_CHILDREN_yes
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	11                              # DW_AT_byte_size
	.byte	11                              # DW_FORM_data1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	8                               # Abbreviation Code
	.byte	40                              # DW_TAG_enumerator
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	28                              # DW_AT_const_value
	.byte	15                              # DW_FORM_udata
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	9                               # Abbreviation Code
	.byte	19                              # DW_TAG_structure_type
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	11                              # DW_AT_byte_size
	.byte	11                              # DW_FORM_data1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	10                              # Abbreviation Code
	.byte	13                              # DW_TAG_member
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	13                              # DW_AT_bit_size
	.byte	11                              # DW_FORM_data1
	.byte	107                             # DW_AT_data_bit_offset
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	11                              # Abbreviation Code
	.byte	23                              # DW_TAG_union_type
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	11                              # DW_AT_byte_size
	.byte	11                              # DW_FORM_data1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	12                              # Abbreviation Code
	.byte	13                              # DW_TAG_member
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	56                              # DW_AT_data_member_location
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	13                              # Abbreviation Code
	.byte	22                              # DW_TAG_typedef
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	14                              # Abbreviation Code
	.byte	21                              # DW_TAG_subroutine_type
	.byte	1                               # DW_CHILDREN_yes
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	15                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	16                              # Abbreviation Code
	.byte	15                              # DW_TAG_pointer_type
	.byte	0                               # DW_CHILDREN_no
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	17                              # Abbreviation Code
	.byte	15                              # DW_TAG_pointer_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	18                              # Abbreviation Code
	.byte	38                              # DW_TAG_const_type
	.byte	0                               # DW_CHILDREN_no
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	19                              # Abbreviation Code
	.byte	19                              # DW_TAG_structure_type
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	60                              # DW_AT_declaration
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	20                              # Abbreviation Code
	.byte	33                              # DW_TAG_subrange_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	21                              # Abbreviation Code
	.byte	38                              # DW_TAG_const_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	22                              # Abbreviation Code
	.byte	19                              # DW_TAG_structure_type
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	11                              # DW_AT_byte_size
	.byte	5                               # DW_FORM_data2
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	23                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	32                              # DW_AT_inline
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	24                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	25                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	26                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	32                              # DW_AT_inline
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	27                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	17                              # DW_AT_low_pc
	.byte	1                               # DW_FORM_addr
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	64                              # DW_AT_frame_base
	.byte	24                              # DW_FORM_exprloc
	.ascii	"\227B"                         # DW_AT_GNU_all_call_sites
	.byte	25                              # DW_FORM_flag_present
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	28                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	23                              # DW_FORM_sec_offset
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	29                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	28                              # DW_AT_const_value
	.byte	13                              # DW_FORM_sdata
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	30                              # Abbreviation Code
	.byte	29                              # DW_TAG_inlined_subroutine
	.byte	1                               # DW_CHILDREN_yes
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	17                              # DW_AT_low_pc
	.byte	1                               # DW_FORM_addr
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	88                              # DW_AT_call_file
	.byte	11                              # DW_FORM_data1
	.byte	89                              # DW_AT_call_line
	.byte	11                              # DW_FORM_data1
	.byte	87                              # DW_AT_call_column
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	31                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	24                              # DW_FORM_exprloc
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	32                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	28                              # DW_AT_const_value
	.byte	15                              # DW_FORM_udata
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	33                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	23                              # DW_FORM_sec_offset
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	34                              # Abbreviation Code
	.byte	10                              # DW_TAG_label
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	17                              # DW_AT_low_pc
	.byte	1                               # DW_FORM_addr
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	35                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	24                              # DW_FORM_exprloc
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	36                              # Abbreviation Code
	.ascii	"\211\202\001"                  # DW_TAG_GNU_call_site
	.byte	1                               # DW_CHILDREN_yes
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	17                              # DW_AT_low_pc
	.byte	1                               # DW_FORM_addr
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	37                              # Abbreviation Code
	.ascii	"\212\202\001"                  # DW_TAG_GNU_call_site_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	24                              # DW_FORM_exprloc
	.ascii	"\221B"                         # DW_AT_GNU_call_site_value
	.byte	24                              # DW_FORM_exprloc
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	38                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	60                              # DW_AT_declaration
	.byte	25                              # DW_FORM_flag_present
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	39                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	14                              # DW_FORM_strp
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	60                              # DW_AT_declaration
	.byte	25                              # DW_FORM_flag_present
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	0                               # EOM(3)
	.section	.debug_info,"",@progbits
.Lcu_begin0:
	.long	.Ldebug_info_end0-.Ldebug_info_start0 # Length of Unit
.Ldebug_info_start0:
	.short	4                               # DWARF version number
	.long	.debug_abbrev                   # Offset Into Abbrev. Section
	.byte	8                               # Address Size (in bytes)
	.byte	1                               # Abbrev [1] 0xb:0xae6 DW_TAG_compile_unit
	.long	.Linfo_string0                  # DW_AT_producer
	.short	12                              # DW_AT_language
	.long	.Linfo_string1                  # DW_AT_name
	.long	.Lline_table_start0             # DW_AT_stmt_list
	.long	.Linfo_string2                  # DW_AT_comp_dir
	.quad	.Lfunc_begin0                   # DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       # DW_AT_high_pc
	.byte	2                               # Abbrev [2] 0x2a:0x11 DW_TAG_variable
	.long	59                              # DW_AT_type
	.byte	1                               # DW_AT_decl_file
	.byte	43                              # DW_AT_decl_line
	.byte	9                               # DW_AT_location
	.byte	3
	.quad	.L.str
	.byte	3                               # Abbrev [3] 0x3b:0xc DW_TAG_array_type
	.long	71                              # DW_AT_type
	.byte	4                               # Abbrev [4] 0x40:0x6 DW_TAG_subrange_type
	.long	78                              # DW_AT_type
	.byte	20                              # DW_AT_count
	.byte	0                               # End Of Children Mark
	.byte	5                               # Abbrev [5] 0x47:0x7 DW_TAG_base_type
	.long	.Linfo_string3                  # DW_AT_name
	.byte	6                               # DW_AT_encoding
	.byte	1                               # DW_AT_byte_size
	.byte	6                               # Abbrev [6] 0x4e:0x7 DW_TAG_base_type
	.long	.Linfo_string4                  # DW_AT_name
	.byte	8                               # DW_AT_byte_size
	.byte	7                               # DW_AT_encoding
	.byte	2                               # Abbrev [2] 0x55:0x11 DW_TAG_variable
	.long	102                             # DW_AT_type
	.byte	1                               # DW_AT_decl_file
	.byte	44                              # DW_AT_decl_line
	.byte	9                               # DW_AT_location
	.byte	3
	.quad	.L.str.1
	.byte	3                               # Abbrev [3] 0x66:0xc DW_TAG_array_type
	.long	71                              # DW_AT_type
	.byte	4                               # Abbrev [4] 0x6b:0x6 DW_TAG_subrange_type
	.long	78                              # DW_AT_type
	.byte	9                               # DW_AT_count
	.byte	0                               # End Of Children Mark
	.byte	2                               # Abbrev [2] 0x72:0x11 DW_TAG_variable
	.long	131                             # DW_AT_type
	.byte	1                               # DW_AT_decl_file
	.byte	44                              # DW_AT_decl_line
	.byte	9                               # DW_AT_location
	.byte	3
	.quad	.L.str.2
	.byte	3                               # Abbrev [3] 0x83:0xc DW_TAG_array_type
	.long	71                              # DW_AT_type
	.byte	4                               # Abbrev [4] 0x88:0x6 DW_TAG_subrange_type
	.long	78                              # DW_AT_type
	.byte	15                              # DW_AT_count
	.byte	0                               # End Of Children Mark
	.byte	2                               # Abbrev [2] 0x8f:0x11 DW_TAG_variable
	.long	160                             # DW_AT_type
	.byte	1                               # DW_AT_decl_file
	.byte	44                              # DW_AT_decl_line
	.byte	9                               # DW_AT_location
	.byte	3
	.quad	.L.str.3
	.byte	3                               # Abbrev [3] 0xa0:0xc DW_TAG_array_type
	.long	71                              # DW_AT_type
	.byte	4                               # Abbrev [4] 0xa5:0x6 DW_TAG_subrange_type
	.long	78                              # DW_AT_type
	.byte	5                               # DW_AT_count
	.byte	0                               # End Of Children Mark
	.byte	2                               # Abbrev [2] 0xac:0x11 DW_TAG_variable
	.long	102                             # DW_AT_type
	.byte	1                               # DW_AT_decl_file
	.byte	45                              # DW_AT_decl_line
	.byte	9                               # DW_AT_location
	.byte	3
	.quad	.L.str.4
	.byte	2                               # Abbrev [2] 0xbd:0x11 DW_TAG_variable
	.long	206                             # DW_AT_type
	.byte	1                               # DW_AT_decl_file
	.byte	46                              # DW_AT_decl_line
	.byte	9                               # DW_AT_location
	.byte	3
	.quad	.L.str.5
	.byte	3                               # Abbrev [3] 0xce:0xc DW_TAG_array_type
	.long	71                              # DW_AT_type
	.byte	4                               # Abbrev [4] 0xd3:0x6 DW_TAG_subrange_type
	.long	78                              # DW_AT_type
	.byte	33                              # DW_AT_count
	.byte	0                               # End Of Children Mark
	.byte	2                               # Abbrev [2] 0xda:0x11 DW_TAG_variable
	.long	235                             # DW_AT_type
	.byte	1                               # DW_AT_decl_file
	.byte	47                              # DW_AT_decl_line
	.byte	9                               # DW_AT_location
	.byte	3
	.quad	.L.str.6
	.byte	3                               # Abbrev [3] 0xeb:0xc DW_TAG_array_type
	.long	71                              # DW_AT_type
	.byte	4                               # Abbrev [4] 0xf0:0x6 DW_TAG_subrange_type
	.long	78                              # DW_AT_type
	.byte	50                              # DW_AT_count
	.byte	0                               # End Of Children Mark
	.byte	7                               # Abbrev [7] 0xf7:0x3d DW_TAG_enumeration_type
	.long	308                             # DW_AT_type
	.long	.Linfo_string14                 # DW_AT_name
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	8                               # Abbrev [8] 0x103:0x6 DW_TAG_enumerator
	.long	.Linfo_string6                  # DW_AT_name
	.byte	0                               # DW_AT_const_value
	.byte	8                               # Abbrev [8] 0x109:0x6 DW_TAG_enumerator
	.long	.Linfo_string7                  # DW_AT_name
	.byte	1                               # DW_AT_const_value
	.byte	8                               # Abbrev [8] 0x10f:0x6 DW_TAG_enumerator
	.long	.Linfo_string8                  # DW_AT_name
	.byte	2                               # DW_AT_const_value
	.byte	8                               # Abbrev [8] 0x115:0x6 DW_TAG_enumerator
	.long	.Linfo_string9                  # DW_AT_name
	.byte	4                               # DW_AT_const_value
	.byte	8                               # Abbrev [8] 0x11b:0x6 DW_TAG_enumerator
	.long	.Linfo_string10                 # DW_AT_name
	.byte	6                               # DW_AT_const_value
	.byte	8                               # Abbrev [8] 0x121:0x6 DW_TAG_enumerator
	.long	.Linfo_string11                 # DW_AT_name
	.byte	8                               # DW_AT_const_value
	.byte	8                               # Abbrev [8] 0x127:0x6 DW_TAG_enumerator
	.long	.Linfo_string12                 # DW_AT_name
	.byte	10                              # DW_AT_const_value
	.byte	8                               # Abbrev [8] 0x12d:0x6 DW_TAG_enumerator
	.long	.Linfo_string13                 # DW_AT_name
	.byte	12                              # DW_AT_const_value
	.byte	0                               # End Of Children Mark
	.byte	5                               # Abbrev [5] 0x134:0x7 DW_TAG_base_type
	.long	.Linfo_string5                  # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	9                               # Abbrev [9] 0x13b:0x23 DW_TAG_structure_type
	.long	.Linfo_string18                 # DW_AT_name
	.byte	8                               # DW_AT_byte_size
	.byte	3                               # DW_AT_decl_file
	.byte	59                              # DW_AT_decl_line
	.byte	10                              # Abbrev [10] 0x143:0xd DW_TAG_member
	.long	.Linfo_string15                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	3                               # DW_AT_decl_file
	.byte	60                              # DW_AT_decl_line
	.byte	16                              # DW_AT_bit_size
	.byte	0                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x150:0xd DW_TAG_member
	.long	.Linfo_string16                 # DW_AT_name
	.long	350                             # DW_AT_type
	.byte	3                               # DW_AT_decl_file
	.byte	61                              # DW_AT_decl_line
	.byte	48                              # DW_AT_bit_size
	.byte	16                              # DW_AT_data_bit_offset
	.byte	0                               # End Of Children Mark
	.byte	5                               # Abbrev [5] 0x15e:0x7 DW_TAG_base_type
	.long	.Linfo_string17                 # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	8                               # DW_AT_byte_size
	.byte	9                               # Abbrev [9] 0x165:0x30 DW_TAG_structure_type
	.long	.Linfo_string22                 # DW_AT_name
	.byte	8                               # DW_AT_byte_size
	.byte	3                               # DW_AT_decl_file
	.byte	59                              # DW_AT_decl_line
	.byte	10                              # Abbrev [10] 0x16d:0xd DW_TAG_member
	.long	.Linfo_string19                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	3                               # DW_AT_decl_file
	.byte	60                              # DW_AT_decl_line
	.byte	16                              # DW_AT_bit_size
	.byte	0                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x17a:0xd DW_TAG_member
	.long	.Linfo_string20                 # DW_AT_name
	.long	350                             # DW_AT_type
	.byte	3                               # DW_AT_decl_file
	.byte	61                              # DW_AT_decl_line
	.byte	44                              # DW_AT_bit_size
	.byte	16                              # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x187:0xd DW_TAG_member
	.long	.Linfo_string21                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	3                               # DW_AT_decl_file
	.byte	62                              # DW_AT_decl_line
	.byte	4                               # DW_AT_bit_size
	.byte	60                              # DW_AT_data_bit_offset
	.byte	0                               # End Of Children Mark
	.byte	11                              # Abbrev [11] 0x195:0x21 DW_TAG_union_type
	.long	.Linfo_string25                 # DW_AT_name
	.byte	8                               # DW_AT_byte_size
	.byte	3                               # DW_AT_decl_file
	.byte	59                              # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x19d:0xc DW_TAG_member
	.long	.Linfo_string23                 # DW_AT_name
	.long	315                             # DW_AT_type
	.byte	3                               # DW_AT_decl_file
	.byte	60                              # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x1a9:0xc DW_TAG_member
	.long	.Linfo_string24                 # DW_AT_name
	.long	357                             # DW_AT_type
	.byte	3                               # DW_AT_decl_file
	.byte	61                              # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x1b6:0x15 DW_TAG_structure_type
	.long	.Linfo_string27                 # DW_AT_name
	.byte	8                               # DW_AT_byte_size
	.byte	3                               # DW_AT_decl_file
	.byte	59                              # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x1be:0xc DW_TAG_member
	.long	.Linfo_string26                 # DW_AT_name
	.long	405                             # DW_AT_type
	.byte	3                               # DW_AT_decl_file
	.byte	60                              # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	13                              # Abbrev [13] 0x1cb:0xb DW_TAG_typedef
	.long	470                             # DW_AT_type
	.long	.Linfo_string28                 # DW_AT_name
	.byte	3                               # DW_AT_decl_file
	.byte	84                              # DW_AT_decl_line
	.byte	14                              # Abbrev [14] 0x1d6:0xb DW_TAG_subroutine_type
	.long	350                             # DW_AT_type
                                        # DW_AT_prototyped
	.byte	15                              # Abbrev [15] 0x1db:0x5 DW_TAG_formal_parameter
	.long	481                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	16                              # Abbrev [16] 0x1e1:0x1 DW_TAG_pointer_type
	.byte	13                              # Abbrev [13] 0x1e2:0xb DW_TAG_typedef
	.long	438                             # DW_AT_type
	.long	.Linfo_string29                 # DW_AT_name
	.byte	3                               # DW_AT_decl_file
	.byte	118                             # DW_AT_decl_line
	.byte	9                               # Abbrev [9] 0x1ed:0x5d DW_TAG_structure_type
	.long	.Linfo_string109                # DW_AT_name
	.byte	64                              # DW_AT_byte_size
	.byte	4                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x1f5:0xc DW_TAG_member
	.long	.Linfo_string30                 # DW_AT_name
	.long	586                             # DW_AT_type
	.byte	4                               # DW_AT_decl_file
	.byte	190                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x201:0xc DW_TAG_member
	.long	.Linfo_string31                 # DW_AT_name
	.long	586                             # DW_AT_type
	.byte	4                               # DW_AT_decl_file
	.byte	191                             # DW_AT_decl_line
	.byte	8                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x20d:0xc DW_TAG_member
	.long	.Linfo_string32                 # DW_AT_name
	.long	592                             # DW_AT_type
	.byte	4                               # DW_AT_decl_file
	.byte	192                             # DW_AT_decl_line
	.byte	16                              # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x219:0xc DW_TAG_member
	.long	.Linfo_string103                # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	4                               # DW_AT_decl_file
	.byte	193                             # DW_AT_decl_line
	.byte	24                              # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x225:0xc DW_TAG_member
	.long	.Linfo_string104                # DW_AT_name
	.long	1607                            # DW_AT_type
	.byte	4                               # DW_AT_decl_file
	.byte	194                             # DW_AT_decl_line
	.byte	28                              # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x231:0xc DW_TAG_member
	.long	.Linfo_string106                # DW_AT_name
	.long	1614                            # DW_AT_type
	.byte	4                               # DW_AT_decl_file
	.byte	195                             # DW_AT_decl_line
	.byte	30                              # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x23d:0xc DW_TAG_member
	.long	.Linfo_string108                # DW_AT_name
	.long	1614                            # DW_AT_type
	.byte	4                               # DW_AT_decl_file
	.byte	196                             # DW_AT_decl_line
	.byte	31                              # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0x24a:0x5 DW_TAG_pointer_type
	.long	591                             # DW_AT_type
	.byte	18                              # Abbrev [18] 0x24f:0x1 DW_TAG_const_type
	.byte	17                              # Abbrev [17] 0x250:0x5 DW_TAG_pointer_type
	.long	597                             # DW_AT_type
	.byte	9                               # Abbrev [9] 0x255:0x45 DW_TAG_structure_type
	.long	.Linfo_string32                 # DW_AT_name
	.byte	24                              # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x25d:0xc DW_TAG_member
	.long	.Linfo_string33                 # DW_AT_name
	.long	666                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x269:0xc DW_TAG_member
	.long	.Linfo_string38                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	8                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x275:0xc DW_TAG_member
	.long	.Linfo_string39                 # DW_AT_name
	.long	714                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	190                             # DW_AT_decl_line
	.byte	12                              # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x281:0xc DW_TAG_member
	.long	.Linfo_string83                 # DW_AT_name
	.long	1308                            # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	191                             # DW_AT_decl_line
	.byte	16                              # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x28d:0xc DW_TAG_member
	.long	.Linfo_string86                 # DW_AT_name
	.long	1380                            # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	192                             # DW_AT_decl_line
	.byte	24                              # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x29a:0x30 DW_TAG_structure_type
	.long	.Linfo_string37                 # DW_AT_name
	.byte	8                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	10                              # Abbrev [10] 0x2a2:0xd DW_TAG_member
	.long	.Linfo_string34                 # DW_AT_name
	.long	350                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	47                              # DW_AT_bit_size
	.byte	0                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x2af:0xd DW_TAG_member
	.long	.Linfo_string35                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	1                               # DW_AT_bit_size
	.byte	47                              # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x2bc:0xd DW_TAG_member
	.long	.Linfo_string36                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	190                             # DW_AT_decl_line
	.byte	16                              # DW_AT_bit_size
	.byte	48                              # DW_AT_data_bit_offset
	.byte	0                               # End Of Children Mark
	.byte	11                              # Abbrev [11] 0x2ca:0x81 DW_TAG_union_type
	.long	.Linfo_string82                 # DW_AT_name
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x2d2:0xc DW_TAG_member
	.long	.Linfo_string40                 # DW_AT_name
	.long	843                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x2de:0xc DW_TAG_member
	.long	.Linfo_string44                 # DW_AT_name
	.long	878                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x2ea:0xc DW_TAG_member
	.long	.Linfo_string46                 # DW_AT_name
	.long	900                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	190                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x2f6:0xc DW_TAG_member
	.long	.Linfo_string53                 # DW_AT_name
	.long	981                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	191                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x302:0xc DW_TAG_member
	.long	.Linfo_string58                 # DW_AT_name
	.long	1042                            # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	192                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x30e:0xc DW_TAG_member
	.long	.Linfo_string62                 # DW_AT_name
	.long	1090                            # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	193                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x31a:0xc DW_TAG_member
	.long	.Linfo_string67                 # DW_AT_name
	.long	1151                            # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	194                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x326:0xc DW_TAG_member
	.long	.Linfo_string73                 # DW_AT_name
	.long	1225                            # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	195                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x332:0xc DW_TAG_member
	.long	.Linfo_string77                 # DW_AT_name
	.long	1273                            # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	196                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x33e:0xc DW_TAG_member
	.long	.Linfo_string81                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	197                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x34b:0x23 DW_TAG_structure_type
	.long	.Linfo_string43                 # DW_AT_name
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	10                              # Abbrev [10] 0x353:0xd DW_TAG_member
	.long	.Linfo_string41                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	4                               # DW_AT_bit_size
	.byte	0                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x360:0xd DW_TAG_member
	.long	.Linfo_string42                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	28                              # DW_AT_bit_size
	.byte	4                               # DW_AT_data_bit_offset
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x36e:0x16 DW_TAG_structure_type
	.long	.Linfo_string45                 # DW_AT_name
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	10                              # Abbrev [10] 0x376:0xd DW_TAG_member
	.long	.Linfo_string41                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	4                               # DW_AT_bit_size
	.byte	0                               # DW_AT_data_bit_offset
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x384:0x4a DW_TAG_structure_type
	.long	.Linfo_string52                 # DW_AT_name
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	10                              # Abbrev [10] 0x38c:0xd DW_TAG_member
	.long	.Linfo_string41                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	4                               # DW_AT_bit_size
	.byte	0                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x399:0xd DW_TAG_member
	.long	.Linfo_string47                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	6                               # DW_AT_bit_size
	.byte	4                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x3a6:0xd DW_TAG_member
	.long	.Linfo_string48                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	190                             # DW_AT_decl_line
	.byte	4                               # DW_AT_bit_size
	.byte	10                              # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x3b3:0xd DW_TAG_member
	.long	.Linfo_string49                 # DW_AT_name
	.long	974                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	191                             # DW_AT_decl_line
	.byte	8                               # DW_AT_bit_size
	.byte	14                              # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x3c0:0xd DW_TAG_member
	.long	.Linfo_string51                 # DW_AT_name
	.long	974                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	192                             # DW_AT_decl_line
	.byte	10                              # DW_AT_bit_size
	.byte	22                              # DW_AT_data_bit_offset
	.byte	0                               # End Of Children Mark
	.byte	5                               # Abbrev [5] 0x3ce:0x7 DW_TAG_base_type
	.long	.Linfo_string50                 # DW_AT_name
	.byte	5                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	9                               # Abbrev [9] 0x3d5:0x3d DW_TAG_structure_type
	.long	.Linfo_string57                 # DW_AT_name
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	10                              # Abbrev [10] 0x3dd:0xd DW_TAG_member
	.long	.Linfo_string41                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	4                               # DW_AT_bit_size
	.byte	0                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x3ea:0xd DW_TAG_member
	.long	.Linfo_string54                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	1                               # DW_AT_bit_size
	.byte	4                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x3f7:0xd DW_TAG_member
	.long	.Linfo_string55                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	190                             # DW_AT_decl_line
	.byte	1                               # DW_AT_bit_size
	.byte	5                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x404:0xd DW_TAG_member
	.long	.Linfo_string56                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	191                             # DW_AT_decl_line
	.byte	26                              # DW_AT_bit_size
	.byte	6                               # DW_AT_data_bit_offset
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x412:0x30 DW_TAG_structure_type
	.long	.Linfo_string61                 # DW_AT_name
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	10                              # Abbrev [10] 0x41a:0xd DW_TAG_member
	.long	.Linfo_string41                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	4                               # DW_AT_bit_size
	.byte	0                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x427:0xd DW_TAG_member
	.long	.Linfo_string59                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	20                              # DW_AT_bit_size
	.byte	4                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x434:0xd DW_TAG_member
	.long	.Linfo_string60                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	190                             # DW_AT_decl_line
	.byte	1                               # DW_AT_bit_size
	.byte	24                              # DW_AT_data_bit_offset
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x442:0x3d DW_TAG_structure_type
	.long	.Linfo_string66                 # DW_AT_name
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	10                              # Abbrev [10] 0x44a:0xd DW_TAG_member
	.long	.Linfo_string41                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	4                               # DW_AT_bit_size
	.byte	0                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x457:0xd DW_TAG_member
	.long	.Linfo_string63                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	5                               # DW_AT_bit_size
	.byte	4                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x464:0xd DW_TAG_member
	.long	.Linfo_string64                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	190                             # DW_AT_decl_line
	.byte	1                               # DW_AT_bit_size
	.byte	9                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x471:0xd DW_TAG_member
	.long	.Linfo_string65                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	191                             # DW_AT_decl_line
	.byte	6                               # DW_AT_bit_size
	.byte	10                              # DW_AT_data_bit_offset
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x47f:0x4a DW_TAG_structure_type
	.long	.Linfo_string72                 # DW_AT_name
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	10                              # Abbrev [10] 0x487:0xd DW_TAG_member
	.long	.Linfo_string41                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	4                               # DW_AT_bit_size
	.byte	0                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x494:0xd DW_TAG_member
	.long	.Linfo_string68                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	10                              # DW_AT_bit_size
	.byte	4                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x4a1:0xd DW_TAG_member
	.long	.Linfo_string69                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	190                             # DW_AT_decl_line
	.byte	10                              # DW_AT_bit_size
	.byte	14                              # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x4ae:0xd DW_TAG_member
	.long	.Linfo_string70                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	191                             # DW_AT_decl_line
	.byte	1                               # DW_AT_bit_size
	.byte	24                              # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x4bb:0xd DW_TAG_member
	.long	.Linfo_string71                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	192                             # DW_AT_decl_line
	.byte	7                               # DW_AT_bit_size
	.byte	25                              # DW_AT_data_bit_offset
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x4c9:0x30 DW_TAG_structure_type
	.long	.Linfo_string76                 # DW_AT_name
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	10                              # Abbrev [10] 0x4d1:0xd DW_TAG_member
	.long	.Linfo_string41                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	4                               # DW_AT_bit_size
	.byte	0                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x4de:0xd DW_TAG_member
	.long	.Linfo_string74                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	14                              # DW_AT_bit_size
	.byte	4                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x4eb:0xd DW_TAG_member
	.long	.Linfo_string75                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	190                             # DW_AT_decl_line
	.byte	14                              # DW_AT_bit_size
	.byte	18                              # DW_AT_data_bit_offset
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x4f9:0x23 DW_TAG_structure_type
	.long	.Linfo_string80                 # DW_AT_name
	.byte	4                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	10                              # Abbrev [10] 0x501:0xd DW_TAG_member
	.long	.Linfo_string78                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	1                               # DW_AT_bit_size
	.byte	0                               # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x50e:0xd DW_TAG_member
	.long	.Linfo_string79                 # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	31                              # DW_AT_bit_size
	.byte	1                               # DW_AT_data_bit_offset
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0x51c:0x5 DW_TAG_pointer_type
	.long	1313                            # DW_AT_type
	.byte	13                              # Abbrev [13] 0x521:0xb DW_TAG_typedef
	.long	1324                            # DW_AT_type
	.long	.Linfo_string85                 # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	14                              # Abbrev [14] 0x52c:0x2e DW_TAG_subroutine_type
	.long	592                             # DW_AT_type
                                        # DW_AT_prototyped
	.byte	15                              # Abbrev [15] 0x531:0x5 DW_TAG_formal_parameter
	.long	592                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0x536:0x5 DW_TAG_formal_parameter
	.long	592                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0x53b:0x5 DW_TAG_formal_parameter
	.long	350                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0x540:0x5 DW_TAG_formal_parameter
	.long	481                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0x545:0x5 DW_TAG_formal_parameter
	.long	481                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0x54a:0x5 DW_TAG_formal_parameter
	.long	350                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0x54f:0x5 DW_TAG_formal_parameter
	.long	481                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0x554:0x5 DW_TAG_formal_parameter
	.long	1370                            # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0x55a:0x5 DW_TAG_pointer_type
	.long	1375                            # DW_AT_type
	.byte	19                              # Abbrev [19] 0x55f:0x5 DW_TAG_structure_type
	.long	.Linfo_string84                 # DW_AT_name
                                        # DW_AT_declaration
	.byte	3                               # Abbrev [3] 0x564:0xb DW_TAG_array_type
	.long	1391                            # DW_AT_type
	.byte	20                              # Abbrev [20] 0x569:0x5 DW_TAG_subrange_type
	.long	78                              # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x56f:0x15 DW_TAG_structure_type
	.long	.Linfo_string102                # DW_AT_name
	.byte	16                              # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x577:0xc DW_TAG_member
	.long	.Linfo_string39                 # DW_AT_name
	.long	1412                            # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	11                              # Abbrev [11] 0x584:0x39 DW_TAG_union_type
	.long	.Linfo_string101                # DW_AT_name
	.byte	16                              # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x58c:0xc DW_TAG_member
	.long	.Linfo_string87                 # DW_AT_name
	.long	1469                            # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x598:0xc DW_TAG_member
	.long	.Linfo_string90                 # DW_AT_name
	.long	1490                            # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x5a4:0xc DW_TAG_member
	.long	.Linfo_string93                 # DW_AT_name
	.long	1511                            # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	190                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x5b0:0xc DW_TAG_member
	.long	.Linfo_string98                 # DW_AT_name
	.long	1571                            # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	191                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x5bd:0x15 DW_TAG_structure_type
	.long	.Linfo_string89                 # DW_AT_name
	.byte	8                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x5c5:0xc DW_TAG_member
	.long	.Linfo_string88                 # DW_AT_name
	.long	592                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x5d2:0x15 DW_TAG_structure_type
	.long	.Linfo_string92                 # DW_AT_name
	.byte	8                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x5da:0xc DW_TAG_member
	.long	.Linfo_string91                 # DW_AT_name
	.long	350                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x5e7:0x3c DW_TAG_structure_type
	.long	.Linfo_string97                 # DW_AT_name
	.byte	16                              # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x5ef:0xc DW_TAG_member
	.long	.Linfo_string88                 # DW_AT_name
	.long	592                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	10                              # Abbrev [10] 0x5fb:0xd DW_TAG_member
	.long	.Linfo_string94                 # DW_AT_name
	.long	350                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	56                              # DW_AT_bit_size
	.byte	64                              # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x608:0xd DW_TAG_member
	.long	.Linfo_string95                 # DW_AT_name
	.long	350                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	190                             # DW_AT_decl_line
	.byte	1                               # DW_AT_bit_size
	.byte	120                             # DW_AT_data_bit_offset
	.byte	10                              # Abbrev [10] 0x615:0xd DW_TAG_member
	.long	.Linfo_string96                 # DW_AT_name
	.long	350                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	191                             # DW_AT_decl_line
	.byte	1                               # DW_AT_bit_size
	.byte	121                             # DW_AT_data_bit_offset
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x623:0x15 DW_TAG_structure_type
	.long	.Linfo_string100                # DW_AT_name
	.byte	8                               # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x62b:0xc DW_TAG_member
	.long	.Linfo_string99                 # DW_AT_name
	.long	1592                            # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0x638:0x5 DW_TAG_pointer_type
	.long	1597                            # DW_AT_type
	.byte	17                              # Abbrev [17] 0x63d:0x5 DW_TAG_pointer_type
	.long	1602                            # DW_AT_type
	.byte	21                              # Abbrev [21] 0x642:0x5 DW_TAG_const_type
	.long	71                              # DW_AT_type
	.byte	5                               # Abbrev [5] 0x647:0x7 DW_TAG_base_type
	.long	.Linfo_string105                # DW_AT_name
	.byte	5                               # DW_AT_encoding
	.byte	2                               # DW_AT_byte_size
	.byte	5                               # Abbrev [5] 0x64e:0x7 DW_TAG_base_type
	.long	.Linfo_string107                # DW_AT_name
	.byte	8                               # DW_AT_encoding
	.byte	1                               # DW_AT_byte_size
	.byte	22                              # Abbrev [22] 0x655:0x52 DW_TAG_structure_type
	.long	.Linfo_string117                # DW_AT_name
	.short	640                             # DW_AT_byte_size
	.byte	4                               # DW_AT_decl_file
	.byte	205                             # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x65e:0xc DW_TAG_member
	.long	.Linfo_string110                # DW_AT_name
	.long	308                             # DW_AT_type
	.byte	4                               # DW_AT_decl_file
	.byte	206                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x66a:0xc DW_TAG_member
	.long	.Linfo_string111                # DW_AT_name
	.long	1703                            # DW_AT_type
	.byte	4                               # DW_AT_decl_file
	.byte	207                             # DW_AT_decl_line
	.byte	4                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x676:0xc DW_TAG_member
	.long	.Linfo_string113                # DW_AT_name
	.long	1708                            # DW_AT_type
	.byte	4                               # DW_AT_decl_file
	.byte	208                             # DW_AT_decl_line
	.byte	6                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x682:0xc DW_TAG_member
	.long	.Linfo_string114                # DW_AT_name
	.long	1614                            # DW_AT_type
	.byte	4                               # DW_AT_decl_file
	.byte	209                             # DW_AT_decl_line
	.byte	8                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x68e:0xc DW_TAG_member
	.long	.Linfo_string115                # DW_AT_name
	.long	1614                            # DW_AT_type
	.byte	4                               # DW_AT_decl_file
	.byte	210                             # DW_AT_decl_line
	.byte	9                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x69a:0xc DW_TAG_member
	.long	.Linfo_string116                # DW_AT_name
	.long	1715                            # DW_AT_type
	.byte	4                               # DW_AT_decl_file
	.byte	211                             # DW_AT_decl_line
	.byte	64                              # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	21                              # Abbrev [21] 0x6a7:0x5 DW_TAG_const_type
	.long	1708                            # DW_AT_type
	.byte	5                               # Abbrev [5] 0x6ac:0x7 DW_TAG_base_type
	.long	.Linfo_string112                # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	2                               # DW_AT_byte_size
	.byte	3                               # Abbrev [3] 0x6b3:0xc DW_TAG_array_type
	.long	493                             # DW_AT_type
	.byte	4                               # Abbrev [4] 0x6b8:0x6 DW_TAG_subrange_type
	.long	78                              # DW_AT_type
	.byte	9                               # DW_AT_count
	.byte	0                               # End Of Children Mark
	.byte	13                              # Abbrev [13] 0x6bf:0xb DW_TAG_typedef
	.long	1738                            # DW_AT_type
	.long	.Linfo_string119                # DW_AT_name
	.byte	5                               # DW_AT_decl_file
	.byte	19                              # DW_AT_decl_line
	.byte	5                               # Abbrev [5] 0x6ca:0x7 DW_TAG_base_type
	.long	.Linfo_string118                # DW_AT_name
	.byte	5                               # DW_AT_encoding
	.byte	8                               # DW_AT_byte_size
	.byte	13                              # Abbrev [13] 0x6d1:0xb DW_TAG_typedef
	.long	350                             # DW_AT_type
	.long	.Linfo_string120                # DW_AT_name
	.byte	6                               # DW_AT_decl_file
	.byte	19                              # DW_AT_decl_line
	.byte	13                              # Abbrev [13] 0x6dc:0xb DW_TAG_typedef
	.long	974                             # DW_AT_type
	.long	.Linfo_string121                # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	25                              # DW_AT_decl_line
	.byte	9                               # Abbrev [9] 0x6e7:0x21 DW_TAG_structure_type
	.long	.Linfo_string124                # DW_AT_name
	.byte	32                              # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x6ef:0xc DW_TAG_member
	.long	.Linfo_string122                # DW_AT_name
	.long	71                              # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x6fb:0xc DW_TAG_member
	.long	.Linfo_string123                # DW_AT_name
	.long	597                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	8                               # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	9                               # Abbrev [9] 0x708:0x21 DW_TAG_structure_type
	.long	.Linfo_string125                # DW_AT_name
	.byte	32                              # DW_AT_byte_size
	.byte	2                               # DW_AT_decl_file
	.byte	187                             # DW_AT_decl_line
	.byte	12                              # Abbrev [12] 0x710:0xc DW_TAG_member
	.long	.Linfo_string122                # DW_AT_name
	.long	71                              # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	188                             # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	12                              # Abbrev [12] 0x71c:0xc DW_TAG_member
	.long	.Linfo_string123                # DW_AT_name
	.long	597                             # DW_AT_type
	.byte	2                               # DW_AT_decl_file
	.byte	189                             # DW_AT_decl_line
	.byte	8                               # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0x729:0x5 DW_TAG_pointer_type
	.long	350                             # DW_AT_type
	.byte	5                               # Abbrev [5] 0x72e:0x7 DW_TAG_base_type
	.long	.Linfo_string126                # DW_AT_name
	.byte	5                               # DW_AT_encoding
	.byte	8                               # DW_AT_byte_size
	.byte	17                              # Abbrev [17] 0x735:0x5 DW_TAG_pointer_type
	.long	71                              # DW_AT_type
	.byte	17                              # Abbrev [17] 0x73a:0x5 DW_TAG_pointer_type
	.long	493                             # DW_AT_type
	.byte	23                              # Abbrev [23] 0x73f:0x7b DW_TAG_subprogram
	.long	.Linfo_string127                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	142                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.long	481                             # DW_AT_type
                                        # DW_AT_external
	.byte	1                               # DW_AT_inline
	.byte	24                              # Abbrev [24] 0x74b:0xb DW_TAG_formal_parameter
	.long	.Linfo_string128                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	142                             # DW_AT_decl_line
	.long	481                             # DW_AT_type
	.byte	24                              # Abbrev [24] 0x756:0xb DW_TAG_formal_parameter
	.long	.Linfo_string129                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	143                             # DW_AT_decl_line
	.long	350                             # DW_AT_type
	.byte	24                              # Abbrev [24] 0x761:0xb DW_TAG_formal_parameter
	.long	.Linfo_string130                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	144                             # DW_AT_decl_line
	.long	350                             # DW_AT_type
	.byte	24                              # Abbrev [24] 0x76c:0xb DW_TAG_formal_parameter
	.long	.Linfo_string131                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	145                             # DW_AT_decl_line
	.long	1833                            # DW_AT_type
	.byte	24                              # Abbrev [24] 0x777:0xb DW_TAG_formal_parameter
	.long	.Linfo_string132                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	146                             # DW_AT_decl_line
	.long	481                             # DW_AT_type
	.byte	25                              # Abbrev [25] 0x782:0xb DW_TAG_variable
	.long	.Linfo_string133                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	149                             # DW_AT_decl_line
	.long	350                             # DW_AT_type
	.byte	25                              # Abbrev [25] 0x78d:0xb DW_TAG_variable
	.long	.Linfo_string134                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	148                             # DW_AT_decl_line
	.long	481                             # DW_AT_type
	.byte	25                              # Abbrev [25] 0x798:0xb DW_TAG_variable
	.long	.Linfo_string135                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	150                             # DW_AT_decl_line
	.long	350                             # DW_AT_type
	.byte	25                              # Abbrev [25] 0x7a3:0xb DW_TAG_variable
	.long	.Linfo_string136                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	151                             # DW_AT_decl_line
	.long	586                             # DW_AT_type
	.byte	25                              # Abbrev [25] 0x7ae:0xb DW_TAG_variable
	.long	.Linfo_string137                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	152                             # DW_AT_decl_line
	.long	586                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	23                              # Abbrev [23] 0x7ba:0x18 DW_TAG_subprogram
	.long	.Linfo_string138                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	107                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.long	586                             # DW_AT_type
                                        # DW_AT_external
	.byte	1                               # DW_AT_inline
	.byte	25                              # Abbrev [25] 0x7c6:0xb DW_TAG_variable
	.long	.Linfo_string139                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	109                             # DW_AT_decl_line
	.long	350                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	26                              # Abbrev [26] 0x7d2:0x1f DW_TAG_subprogram
	.long	.Linfo_string140                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	100                             # DW_AT_decl_line
                                        # DW_AT_prototyped
                                        # DW_AT_external
	.byte	1                               # DW_AT_inline
	.byte	24                              # Abbrev [24] 0x7da:0xb DW_TAG_formal_parameter
	.long	.Linfo_string141                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	100                             # DW_AT_decl_line
	.long	481                             # DW_AT_type
	.byte	25                              # Abbrev [25] 0x7e5:0xb DW_TAG_variable
	.long	.Linfo_string136                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	102                             # DW_AT_decl_line
	.long	481                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	27                              # Abbrev [27] 0x7f1:0x284 DW_TAG_subprogram
	.quad	.Lfunc_begin0                   # DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	86
                                        # DW_AT_GNU_all_call_sites
	.long	.Linfo_string147                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	39                              # DW_AT_decl_line
                                        # DW_AT_prototyped
	.long	974                             # DW_AT_type
                                        # DW_AT_external
	.byte	28                              # Abbrev [28] 0x80a:0xf DW_TAG_variable
	.long	.Ldebug_loc0                    # DW_AT_location
	.long	.Linfo_string148                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	47                              # DW_AT_decl_line
	.long	350                             # DW_AT_type
	.byte	29                              # Abbrev [29] 0x819:0xd DW_TAG_variable
	.ascii	"\320\001"                      # DW_AT_const_value
	.long	.Linfo_string149                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	49                              # DW_AT_decl_line
	.long	1738                            # DW_AT_type
	.byte	28                              # Abbrev [28] 0x826:0xf DW_TAG_variable
	.long	.Ldebug_loc1                    # DW_AT_location
	.long	.Linfo_string150                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	48                              # DW_AT_decl_line
	.long	481                             # DW_AT_type
	.byte	28                              # Abbrev [28] 0x835:0xf DW_TAG_variable
	.long	.Ldebug_loc2                    # DW_AT_location
	.long	.Linfo_string151                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	50                              # DW_AT_decl_line
	.long	481                             # DW_AT_type
	.byte	28                              # Abbrev [28] 0x844:0xf DW_TAG_variable
	.long	.Ldebug_loc5                    # DW_AT_location
	.long	.Linfo_string136                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	42                              # DW_AT_decl_line
	.long	481                             # DW_AT_type
	.byte	28                              # Abbrev [28] 0x853:0xf DW_TAG_variable
	.long	.Ldebug_loc6                    # DW_AT_location
	.long	.Linfo_string152                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	41                              # DW_AT_decl_line
	.long	481                             # DW_AT_type
	.byte	28                              # Abbrev [28] 0x862:0xf DW_TAG_variable
	.long	.Ldebug_loc11                   # DW_AT_location
	.long	.Linfo_string137                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	44                              # DW_AT_decl_line
	.long	592                             # DW_AT_type
	.byte	28                              # Abbrev [28] 0x871:0xf DW_TAG_variable
	.long	.Ldebug_loc12                   # DW_AT_location
	.long	.Linfo_string153                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	43                              # DW_AT_decl_line
	.long	592                             # DW_AT_type
	.byte	25                              # Abbrev [25] 0x880:0xb DW_TAG_variable
	.long	.Linfo_string162                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	45                              # DW_AT_decl_line
	.long	592                             # DW_AT_type
	.byte	25                              # Abbrev [25] 0x88b:0xb DW_TAG_variable
	.long	.Linfo_string163                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	46                              # DW_AT_decl_line
	.long	481                             # DW_AT_type
	.byte	30                              # Abbrev [30] 0x896:0x88 DW_TAG_inlined_subroutine
	.long	1855                            # DW_AT_abstract_origin
	.quad	.Ltmp2                          # DW_AT_low_pc
	.long	.Ltmp7-.Ltmp2                   # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	41                              # DW_AT_call_line
	.byte	9                               # DW_AT_call_column
	.byte	31                              # Abbrev [31] 0x8aa:0xa DW_TAG_formal_parameter
	.byte	4                               # DW_AT_location
	.byte	145
	.ascii	"\220~"
	.byte	159
	.long	1867                            # DW_AT_abstract_origin
	.byte	32                              # Abbrev [32] 0x8b4:0x7 DW_TAG_formal_parameter
	.ascii	"\250\001"                      # DW_AT_const_value
	.long	1878                            # DW_AT_abstract_origin
	.byte	32                              # Abbrev [32] 0x8bb:0x7 DW_TAG_formal_parameter
	.ascii	"\320\001"                      # DW_AT_const_value
	.long	1889                            # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x8c2:0x9 DW_TAG_formal_parameter
	.byte	3                               # DW_AT_location
	.byte	145
	.byte	104
	.byte	159
	.long	1900                            # DW_AT_abstract_origin
	.byte	31                              # Abbrev [31] 0x8cb:0x7 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	82
	.long	1911                            # DW_AT_abstract_origin
	.byte	33                              # Abbrev [33] 0x8d2:0x9 DW_TAG_variable
	.long	.Ldebug_loc3                    # DW_AT_location
	.long	1922                            # DW_AT_abstract_origin
	.byte	33                              # Abbrev [33] 0x8db:0x9 DW_TAG_variable
	.long	.Ldebug_loc4                    # DW_AT_location
	.long	1933                            # DW_AT_abstract_origin
	.byte	33                              # Abbrev [33] 0x8e4:0x9 DW_TAG_variable
	.long	.Ldebug_loc7                    # DW_AT_location
	.long	1944                            # DW_AT_abstract_origin
	.byte	33                              # Abbrev [33] 0x8ed:0x9 DW_TAG_variable
	.long	.Ldebug_loc8                    # DW_AT_location
	.long	1955                            # DW_AT_abstract_origin
	.byte	33                              # Abbrev [33] 0x8f6:0x9 DW_TAG_variable
	.long	.Ldebug_loc10                   # DW_AT_location
	.long	1966                            # DW_AT_abstract_origin
	.byte	30                              # Abbrev [30] 0x8ff:0x1e DW_TAG_inlined_subroutine
	.long	1978                            # DW_AT_abstract_origin
	.quad	.Ltmp4                          # DW_AT_low_pc
	.long	.Ltmp6-.Ltmp4                   # DW_AT_high_pc
	.byte	4                               # DW_AT_call_file
	.byte	168                             # DW_AT_call_line
	.byte	13                              # DW_AT_call_column
	.byte	33                              # Abbrev [33] 0x913:0x9 DW_TAG_variable
	.long	.Ldebug_loc9                    # DW_AT_location
	.long	1990                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	34                              # Abbrev [34] 0x91e:0xf DW_TAG_label
	.long	.Linfo_string155                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	46                              # DW_AT_decl_line
	.quad	.Ltmp15                         # DW_AT_low_pc
	.byte	34                              # Abbrev [34] 0x92d:0xf DW_TAG_label
	.long	.Linfo_string154                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	45                              # DW_AT_decl_line
	.quad	.Ltmp11                         # DW_AT_low_pc
	.byte	34                              # Abbrev [34] 0x93c:0xf DW_TAG_label
	.long	.Linfo_string157                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	47                              # DW_AT_decl_line
	.quad	.Ltmp17                         # DW_AT_low_pc
	.byte	34                              # Abbrev [34] 0x94b:0xf DW_TAG_label
	.long	.Linfo_string156                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	46                              # DW_AT_decl_line
	.quad	.Ltmp15                         # DW_AT_low_pc
	.byte	34                              # Abbrev [34] 0x95a:0xf DW_TAG_label
	.long	.Linfo_string159                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	48                              # DW_AT_decl_line
	.quad	.Ltmp20                         # DW_AT_low_pc
	.byte	34                              # Abbrev [34] 0x969:0xf DW_TAG_label
	.long	.Linfo_string158                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	47                              # DW_AT_decl_line
	.quad	.Ltmp17                         # DW_AT_low_pc
	.byte	34                              # Abbrev [34] 0x978:0xf DW_TAG_label
	.long	.Linfo_string161                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	49                              # DW_AT_decl_line
	.quad	.Ltmp24                         # DW_AT_low_pc
	.byte	34                              # Abbrev [34] 0x987:0xf DW_TAG_label
	.long	.Linfo_string160                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	48                              # DW_AT_decl_line
	.quad	.Ltmp20                         # DW_AT_low_pc
	.byte	30                              # Abbrev [30] 0x996:0x25 DW_TAG_inlined_subroutine
	.long	2002                            # DW_AT_abstract_origin
	.quad	.Ltmp25                         # DW_AT_low_pc
	.long	.Ltmp26-.Ltmp25                 # DW_AT_high_pc
	.byte	1                               # DW_AT_call_file
	.byte	51                              # DW_AT_call_line
	.byte	1                               # DW_AT_call_column
	.byte	31                              # Abbrev [31] 0x9aa:0x9 DW_TAG_formal_parameter
	.byte	3                               # DW_AT_location
	.byte	145
	.byte	104
	.byte	159
	.long	2010                            # DW_AT_abstract_origin
	.byte	35                              # Abbrev [35] 0x9b3:0x7 DW_TAG_variable
	.byte	1                               # DW_AT_location
	.byte	86
	.long	2021                            # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	36                              # Abbrev [36] 0x9bb:0x27 DW_TAG_GNU_call_site
	.long	2677                            # DW_AT_abstract_origin
	.quad	.Ltmp7                          # DW_AT_low_pc
	.byte	37                              # Abbrev [37] 0x9c8:0x6 DW_TAG_GNU_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	89
	.byte	2                               # DW_AT_GNU_call_site_value
	.byte	145
	.byte	0
	.byte	37                              # Abbrev [37] 0x9ce:0x6 DW_TAG_GNU_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	85
	.byte	2                               # DW_AT_GNU_call_site_value
	.byte	115
	.byte	0
	.byte	37                              # Abbrev [37] 0x9d4:0x7 DW_TAG_GNU_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	84
	.byte	3                               # DW_AT_GNU_call_site_value
	.byte	16
	.ascii	"\250\001"
	.byte	37                              # Abbrev [37] 0x9db:0x6 DW_TAG_GNU_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	81
	.byte	2                               # DW_AT_GNU_call_site_value
	.byte	145
	.byte	104
	.byte	0                               # End Of Children Mark
	.byte	36                              # Abbrev [36] 0x9e2:0x14 DW_TAG_GNU_call_site
	.long	2715                            # DW_AT_abstract_origin
	.quad	.Ltmp8                          # DW_AT_low_pc
	.byte	37                              # Abbrev [37] 0x9ef:0x6 DW_TAG_GNU_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	85
	.byte	2                               # DW_AT_GNU_call_site_value
	.byte	115
	.byte	0
	.byte	0                               # End Of Children Mark
	.byte	36                              # Abbrev [36] 0x9f6:0x14 DW_TAG_GNU_call_site
	.long	2732                            # DW_AT_abstract_origin
	.quad	.Ltmp10                         # DW_AT_low_pc
	.byte	37                              # Abbrev [37] 0xa03:0x6 DW_TAG_GNU_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	85
	.byte	2                               # DW_AT_GNU_call_site_value
	.byte	48
	.byte	32
	.byte	0                               # End Of Children Mark
	.byte	36                              # Abbrev [36] 0xa0a:0x14 DW_TAG_GNU_call_site
	.long	2754                            # DW_AT_abstract_origin
	.quad	.Ltmp15                         # DW_AT_low_pc
	.byte	37                              # Abbrev [37] 0xa17:0x6 DW_TAG_GNU_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	81
	.byte	2                               # DW_AT_GNU_call_site_value
	.byte	16
	.byte	38
	.byte	0                               # End Of Children Mark
	.byte	36                              # Abbrev [36] 0xa1e:0x14 DW_TAG_GNU_call_site
	.long	2754                            # DW_AT_abstract_origin
	.quad	.Ltmp17                         # DW_AT_low_pc
	.byte	37                              # Abbrev [37] 0xa2b:0x6 DW_TAG_GNU_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	81
	.byte	2                               # DW_AT_GNU_call_site_value
	.byte	16
	.byte	39
	.byte	0                               # End Of Children Mark
	.byte	36                              # Abbrev [36] 0xa32:0x14 DW_TAG_GNU_call_site
	.long	2754                            # DW_AT_abstract_origin
	.quad	.Ltmp19                         # DW_AT_low_pc
	.byte	37                              # Abbrev [37] 0xa3f:0x6 DW_TAG_GNU_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	81
	.byte	2                               # DW_AT_GNU_call_site_value
	.byte	16
	.byte	40
	.byte	0                               # End Of Children Mark
	.byte	36                              # Abbrev [36] 0xa46:0x14 DW_TAG_GNU_call_site
	.long	2754                            # DW_AT_abstract_origin
	.quad	.Ltmp24                         # DW_AT_low_pc
	.byte	37                              # Abbrev [37] 0xa53:0x6 DW_TAG_GNU_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	81
	.byte	2                               # DW_AT_GNU_call_site_value
	.byte	16
	.byte	41
	.byte	0                               # End Of Children Mark
	.byte	36                              # Abbrev [36] 0xa5a:0x1a DW_TAG_GNU_call_site
	.long	2782                            # DW_AT_abstract_origin
	.quad	.Ltmp26                         # DW_AT_low_pc
	.byte	37                              # Abbrev [37] 0xa67:0x6 DW_TAG_GNU_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	84
	.byte	2                               # DW_AT_GNU_call_site_value
	.byte	145
	.byte	0
	.byte	37                              # Abbrev [37] 0xa6d:0x6 DW_TAG_GNU_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	85
	.byte	2                               # DW_AT_GNU_call_site_value
	.byte	145
	.byte	104
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	38                              # Abbrev [38] 0xa75:0x26 DW_TAG_subprogram
	.long	.Linfo_string142                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	48                              # DW_AT_decl_line
                                        # DW_AT_prototyped
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	15                              # Abbrev [15] 0xa7c:0x5 DW_TAG_formal_parameter
	.long	481                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0xa81:0x5 DW_TAG_formal_parameter
	.long	350                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0xa86:0x5 DW_TAG_formal_parameter
	.long	1833                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0xa8b:0x5 DW_TAG_formal_parameter
	.long	586                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0xa90:0x5 DW_TAG_formal_parameter
	.long	586                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0xa95:0x5 DW_TAG_formal_parameter
	.long	586                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	39                              # Abbrev [39] 0xa9b:0x11 DW_TAG_subprogram
	.long	.Linfo_string143                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	6                               # DW_AT_decl_line
                                        # DW_AT_prototyped
	.long	592                             # DW_AT_type
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	15                              # Abbrev [15] 0xaa6:0x5 DW_TAG_formal_parameter
	.long	481                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	39                              # Abbrev [39] 0xaac:0x16 DW_TAG_subprogram
	.long	.Linfo_string144                # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	7                               # DW_AT_decl_line
                                        # DW_AT_prototyped
	.long	481                             # DW_AT_type
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	15                              # Abbrev [15] 0xab7:0x5 DW_TAG_formal_parameter
	.long	481                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0xabc:0x5 DW_TAG_formal_parameter
	.long	1597                            # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	38                              # Abbrev [38] 0xac2:0x1c DW_TAG_subprogram
	.long	.Linfo_string145                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	46                              # DW_AT_decl_line
                                        # DW_AT_prototyped
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	15                              # Abbrev [15] 0xac9:0x5 DW_TAG_formal_parameter
	.long	1597                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0xace:0x5 DW_TAG_formal_parameter
	.long	1597                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0xad3:0x5 DW_TAG_formal_parameter
	.long	308                             # DW_AT_type
	.byte	15                              # Abbrev [15] 0xad8:0x5 DW_TAG_formal_parameter
	.long	1597                            # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	38                              # Abbrev [38] 0xade:0x12 DW_TAG_subprogram
	.long	.Linfo_string146                # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.byte	97                              # DW_AT_decl_line
                                        # DW_AT_prototyped
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	15                              # Abbrev [15] 0xae5:0x5 DW_TAG_formal_parameter
	.long	1833                            # DW_AT_type
	.byte	15                              # Abbrev [15] 0xaea:0x5 DW_TAG_formal_parameter
	.long	481                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"Ubuntu clang version 20.1.8 (0ubuntu4)" # string offset=0
.Linfo_string1:
	.asciz	"alloca-clang.c"                # string offset=39
.Linfo_string2:
	.asciz	"/home/zoltan/Develop/liballocs/tests/alloca-clang" # string offset=54
.Linfo_string3:
	.asciz	"char"                          # string offset=104
.Linfo_string4:
	.asciz	"__ARRAY_SIZE_TYPE__"           # string offset=109
.Linfo_string5:
	.asciz	"unsigned int"                  # string offset=129
.Linfo_string6:
	.asciz	"VOID"                          # string offset=142
.Linfo_string7:
	.asciz	"ARRAY"                         # string offset=147
.Linfo_string8:
	.asciz	"BASE"                          # string offset=153
.Linfo_string9:
	.asciz	"ENUMERATION"                   # string offset=158
.Linfo_string10:
	.asciz	"COMPOSITE"                     # string offset=170
.Linfo_string11:
	.asciz	"ADDRESS"                       # string offset=180
.Linfo_string12:
	.asciz	"SUBPROGRAM"                    # string offset=188
.Linfo_string13:
	.asciz	"SUBRANGE"                      # string offset=199
.Linfo_string14:
	.asciz	"uniqtype_kind"                 # string offset=208
.Linfo_string15:
	.asciz	"unused"                        # string offset=222
.Linfo_string16:
	.asciz	"alloc_site"                    # string offset=229
.Linfo_string17:
	.asciz	"unsigned long"                 # string offset=240
.Linfo_string18:
	.asciz	"__anonstruct_initial_884576843" # string offset=254
.Linfo_string19:
	.asciz	"alloc_site_id"                 # string offset=285
.Linfo_string20:
	.asciz	"uniqtype_shifted"              # string offset=299
.Linfo_string21:
	.asciz	"lifetime_policies"             # string offset=316
.Linfo_string22:
	.asciz	"insert_with_type"              # string offset=334
.Linfo_string23:
	.asciz	"initial"                       # string offset=351
.Linfo_string24:
	.asciz	"with_type"                     # string offset=359
.Linfo_string25:
	.asciz	"__anonunion____missing_field_name_884576842" # string offset=369
.Linfo_string26:
	.asciz	"__annonCompField1"             # string offset=413
.Linfo_string27:
	.asciz	"insert"                        # string offset=431
.Linfo_string28:
	.asciz	"sizefn_t"                      # string offset=438
.Linfo_string29:
	.asciz	"lifetime_insert_t"             # string offset=447
.Linfo_string30:
	.asciz	"obj_base"                      # string offset=465
.Linfo_string31:
	.asciz	"obj_limit"                     # string offset=474
.Linfo_string32:
	.asciz	"uniqtype"                      # string offset=484
.Linfo_string33:
	.asciz	"cache_word"                    # string offset=493
.Linfo_string34:
	.asciz	"addr"                          # string offset=504
.Linfo_string35:
	.asciz	"flag"                          # string offset=509
.Linfo_string36:
	.asciz	"bits"                          # string offset=514
.Linfo_string37:
	.asciz	"alloc_addr_info"               # string offset=519
.Linfo_string38:
	.asciz	"pos_maxoff"                    # string offset=535
.Linfo_string39:
	.asciz	"un"                            # string offset=546
.Linfo_string40:
	.asciz	"info"                          # string offset=549
.Linfo_string41:
	.asciz	"kind"                          # string offset=554
.Linfo_string42:
	.asciz	"unused_"                       # string offset=559
.Linfo_string43:
	.asciz	"__anonstruct_info_833258354"   # string offset=567
.Linfo_string44:
	.asciz	"_void"                         # string offset=595
.Linfo_string45:
	.asciz	"__anonstruct__void_833258355"  # string offset=601
.Linfo_string46:
	.asciz	"base"                          # string offset=630
.Linfo_string47:
	.asciz	"enc"                           # string offset=635
.Linfo_string48:
	.asciz	"one_plus_log_bit_size_delta"   # string offset=639
.Linfo_string49:
	.asciz	"bit_size_delta_delta"          # string offset=667
.Linfo_string50:
	.asciz	"int"                           # string offset=688
.Linfo_string51:
	.asciz	"bit_off"                       # string offset=692
.Linfo_string52:
	.asciz	"__anonstruct_base_833258356"   # string offset=700
.Linfo_string53:
	.asciz	"enumeration"                   # string offset=728
.Linfo_string54:
	.asciz	"is_contiguous"                 # string offset=740
.Linfo_string55:
	.asciz	"is_log_spaced"                 # string offset=754
.Linfo_string56:
	.asciz	"nenum"                         # string offset=768
.Linfo_string57:
	.asciz	"__anonstruct_enumeration_833258357" # string offset=774
.Linfo_string58:
	.asciz	"composite"                     # string offset=809
.Linfo_string59:
	.asciz	"nmemb"                         # string offset=819
.Linfo_string60:
	.asciz	"not_simultaneous"              # string offset=825
.Linfo_string61:
	.asciz	"__anonstruct_composite_833258358" # string offset=842
.Linfo_string62:
	.asciz	"address"                       # string offset=875
.Linfo_string63:
	.asciz	"indir_level"                   # string offset=883
.Linfo_string64:
	.asciz	"genericity"                    # string offset=895
.Linfo_string65:
	.asciz	"log_min_align"                 # string offset=906
.Linfo_string66:
	.asciz	"__anonstruct_address_833258359" # string offset=920
.Linfo_string67:
	.asciz	"subprogram"                    # string offset=951
.Linfo_string68:
	.asciz	"narg"                          # string offset=962
.Linfo_string69:
	.asciz	"nret"                          # string offset=967
.Linfo_string70:
	.asciz	"is_va"                         # string offset=972
.Linfo_string71:
	.asciz	"cc"                            # string offset=978
.Linfo_string72:
	.asciz	"__anonstruct_subprogram_833258360" # string offset=981
.Linfo_string73:
	.asciz	"subrange"                      # string offset=1015
.Linfo_string74:
	.asciz	"min"                           # string offset=1024
.Linfo_string75:
	.asciz	"max"                           # string offset=1028
.Linfo_string76:
	.asciz	"__anonstruct_subrange_833258361" # string offset=1032
.Linfo_string77:
	.asciz	"array"                         # string offset=1064
.Linfo_string78:
	.asciz	"is_array"                      # string offset=1070
.Linfo_string79:
	.asciz	"nelems"                        # string offset=1079
.Linfo_string80:
	.asciz	"__anonstruct_array_833258362"  # string offset=1086
.Linfo_string81:
	.asciz	"as_word"                       # string offset=1115
.Linfo_string82:
	.asciz	"__anonunion_un_833258353"      # string offset=1123
.Linfo_string83:
	.asciz	"make_precise"                  # string offset=1148
.Linfo_string84:
	.asciz	"mcontext"                      # string offset=1161
.Linfo_string85:
	.asciz	"make_precise_fn_t"             # string offset=1170
.Linfo_string86:
	.asciz	"related"                       # string offset=1188
.Linfo_string87:
	.asciz	"t"                             # string offset=1196
.Linfo_string88:
	.asciz	"ptr"                           # string offset=1198
.Linfo_string89:
	.asciz	"__anonstruct_t_833258349"      # string offset=1202
.Linfo_string90:
	.asciz	"enumerator"                    # string offset=1227
.Linfo_string91:
	.asciz	"val"                           # string offset=1238
.Linfo_string92:
	.asciz	"__anonstruct_enumerator_833258350" # string offset=1242
.Linfo_string93:
	.asciz	"memb"                          # string offset=1276
.Linfo_string94:
	.asciz	"off"                           # string offset=1281
.Linfo_string95:
	.asciz	"is_absolute_address"           # string offset=1285
.Linfo_string96:
	.asciz	"may_be_invalid"                # string offset=1305
.Linfo_string97:
	.asciz	"__anonstruct_memb_833258351"   # string offset=1320
.Linfo_string98:
	.asciz	"memb_names"                    # string offset=1348
.Linfo_string99:
	.asciz	"n"                             # string offset=1359
.Linfo_string100:
	.asciz	"__anonstruct_memb_names_833258352" # string offset=1361
.Linfo_string101:
	.asciz	"__anonunion_un_833258348"      # string offset=1395
.Linfo_string102:
	.asciz	"uniqtype_rel_info"             # string offset=1420
.Linfo_string103:
	.asciz	"period"                        # string offset=1438
.Linfo_string104:
	.asciz	"depth"                         # string offset=1445
.Linfo_string105:
	.asciz	"short"                         # string offset=1451
.Linfo_string106:
	.asciz	"prev_mru"                      # string offset=1457
.Linfo_string107:
	.asciz	"unsigned char"                 # string offset=1466
.Linfo_string108:
	.asciz	"next_mru"                      # string offset=1480
.Linfo_string109:
	.asciz	"__liballocs_memrange_cache_entry_s" # string offset=1489
.Linfo_string110:
	.asciz	"validity"                      # string offset=1524
.Linfo_string111:
	.asciz	"size_plus_one"                 # string offset=1533
.Linfo_string112:
	.asciz	"unsigned short"                # string offset=1547
.Linfo_string113:
	.asciz	"next_victim"                   # string offset=1562
.Linfo_string114:
	.asciz	"head_mru"                      # string offset=1574
.Linfo_string115:
	.asciz	"tail_mru"                      # string offset=1583
.Linfo_string116:
	.asciz	"entries"                       # string offset=1592
.Linfo_string117:
	.asciz	"__liballocs_memrange_cache"    # string offset=1600
.Linfo_string118:
	.asciz	"long"                          # string offset=1627
.Linfo_string119:
	.asciz	"ptrdiff_t"                     # string offset=1632
.Linfo_string120:
	.asciz	"size_t"                        # string offset=1642
.Linfo_string121:
	.asciz	"wchar_t"                       # string offset=1649
.Linfo_string122:
	.asciz	"c"                             # string offset=1657
.Linfo_string123:
	.asciz	"member"                        # string offset=1659
.Linfo_string124:
	.asciz	"__anonstruct_833258363"        # string offset=1666
.Linfo_string125:
	.asciz	"__anonstruct_833258364"        # string offset=1689
.Linfo_string126:
	.asciz	"long long"                     # string offset=1712
.Linfo_string127:
	.asciz	"__liballocs_notify_and_adjust_alloca" # string offset=1722
.Linfo_string128:
	.asciz	"allocated"                     # string offset=1759
.Linfo_string129:
	.asciz	"orig_size"                     # string offset=1769
.Linfo_string130:
	.asciz	"tweaked_size"                  # string offset=1779
.Linfo_string131:
	.asciz	"frame_counter"                 # string offset=1792
.Linfo_string132:
	.asciz	"caller"                        # string offset=1806
.Linfo_string133:
	.asciz	"header_effective_size"         # string offset=1813
.Linfo_string134:
	.asciz	"userchunk"                     # string offset=1835
.Linfo_string135:
	.asciz	"non_header_size"               # string offset=1845
.Linfo_string136:
	.asciz	"tmp"                           # string offset=1861
.Linfo_string137:
	.asciz	"tmp___0"                       # string offset=1865
.Linfo_string138:
	.asciz	"__liballocs_get_sp"            # string offset=1873
.Linfo_string139:
	.asciz	"our_sp"                        # string offset=1892
.Linfo_string140:
	.asciz	"__liballocs_alloca_caller_frame_cleanup" # string offset=1899
.Linfo_string141:
	.asciz	"counter"                       # string offset=1939
.Linfo_string142:
	.asciz	"__alloca_allocator_notify"     # string offset=1947
.Linfo_string143:
	.asciz	"__liballocs_get_alloc_type"    # string offset=1973
.Linfo_string144:
	.asciz	"dlsym"                         # string offset=2000
.Linfo_string145:
	.asciz	"__assert_fail"                 # string offset=2006
.Linfo_string146:
	.asciz	"__liballocs_unindex_stack_objects_counted_by" # string offset=2020
.Linfo_string147:
	.asciz	"main"                          # string offset=2065
.Linfo_string148:
	.asciz	"__liballocs_alloca_cleanup_local" # string offset=2070
.Linfo_string149:
	.asciz	"__cil_tmp9"                    # string offset=2103
.Linfo_string150:
	.asciz	"__cil_tmp8"                    # string offset=2114
.Linfo_string151:
	.asciz	"__cil_tmp10"                   # string offset=2125
.Linfo_string152:
	.asciz	"a"                             # string offset=2137
.Linfo_string153:
	.asciz	"got_type"                      # string offset=2139
.Linfo_string154:
	.asciz	"while_continue"                # string offset=2148
.Linfo_string155:
	.asciz	"while_break"                   # string offset=2163
.Linfo_string156:
	.asciz	"while_continue___0"            # string offset=2175
.Linfo_string157:
	.asciz	"while_break___0"               # string offset=2194
.Linfo_string158:
	.asciz	"while_continue___1"            # string offset=2210
.Linfo_string159:
	.asciz	"while_break___1"               # string offset=2229
.Linfo_string160:
	.asciz	"while_continue___2"            # string offset=2245
.Linfo_string161:
	.asciz	"while_break___2"               # string offset=2264
.Linfo_string162:
	.asciz	"int_type"                      # string offset=2280
.Linfo_string163:
	.asciz	"tmp___1"                       # string offset=2289
	.ident	"Ubuntu clang version 20.1.8 (0ubuntu4)"
	.section	".note.GNU-stack","",@progbits
	.section	.debug_line,"",@progbits
.Lline_table_start0:
