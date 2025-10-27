	.file	"construct"
	.text
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%r12
	.cfi_def_cfa_offset 40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -48
	.cfi_offset %r12, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movl	$10, %edi
	movl	$5, %esi
	callq	add@PLT
	movl	%eax, %ebx
	movl	$4, %edi
	movl	$3, %esi
	callq	multiply@PLT
	movl	%eax, %ebp
	movl	$7, %edi
	callq	square@PLT
	movl	%eax, %r14d
	movl	$42, %edi
	callq	is_positive@PLT
	movl	%eax, %r15d
	movl	$-5, %edi
	callq	is_positive@PLT
	movl	%eax, %r12d
	movl	$.Lstr, %edi
	callq	construct_dump_string@PLT
	movl	%ebx, %edi
	callq	construct_dump_int@PLT
	movl	%ebp, %edi
	callq	construct_dump_int@PLT
	movl	%r14d, %edi
	callq	construct_dump_int@PLT
	movzbl	%r15b, %edi
	callq	construct_dump_bool@PLT
	movzbl	%r12b, %edi
	callq	construct_dump_bool@PLT
	movl	$.Lstr.2, %edi
	callq	construct_dump_string@PLT
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r14
	.cfi_def_cfa_offset 24
	popq	%r15
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.globl	add                             # -- Begin function add
	.p2align	4
	.type	add,@function
add:                                    # @add
	.cfi_startproc
# %bb.0:                                # %entry
                                        # kill: def $esi killed $esi def $rsi
                                        # kill: def $edi killed $edi def $rdi
	leal	(%rdi,%rsi), %eax
	retq
.Lfunc_end1:
	.size	add, .Lfunc_end1-add
	.cfi_endproc
                                        # -- End function
	.globl	multiply                        # -- Begin function multiply
	.p2align	4
	.type	multiply,@function
multiply:                               # @multiply
	.cfi_startproc
# %bb.0:                                # %entry
	movl	%edi, %eax
	imull	%esi, %eax
	retq
.Lfunc_end2:
	.size	multiply, .Lfunc_end2-multiply
	.cfi_endproc
                                        # -- End function
	.globl	square                          # -- Begin function square
	.p2align	4
	.type	square,@function
square:                                 # @square
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	%edi, %esi
	callq	multiply@PLT
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end3:
	.size	square, .Lfunc_end3-square
	.cfi_endproc
                                        # -- End function
	.globl	is_positive                     # -- Begin function is_positive
	.p2align	4
	.type	is_positive,@function
is_positive:                            # @is_positive
	.cfi_startproc
# %bb.0:                                # %entry
	testl	%edi, %edi
	setg	%al
	retq
.Lfunc_end4:
	.size	is_positive, .Lfunc_end4-is_positive
	.cfi_endproc
                                        # -- End function
	.type	.Lstr,@object                   # @str
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"adult"
	.size	.Lstr, 6

	.type	.Lstr.1,@object                 # @str.1
.Lstr.1:
	.asciz	"minor"
	.size	.Lstr.1, 6

	.type	.Lstr.2,@object                 # @str.2
.Lstr.2:
	.asciz	"one"
	.size	.Lstr.2, 4

	.type	.Lstr.3,@object                 # @str.3
.Lstr.3:
	.asciz	"two"
	.size	.Lstr.3, 4

	.type	.Lstr.4,@object                 # @str.4
.Lstr.4:
	.asciz	"other"
	.size	.Lstr.4, 6

	.section	".note.GNU-stack","",@progbits
