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
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	.cfi_offset %rbp, -16
	movl	$5, %edi
	movl	$3, %esi
	callq	add@PLT
	movl	%eax, %ebx
	movl	$4, %edi
	movl	$7, %esi
	callq	multiply@PLT
	movl	%eax, %ebp
	movl	$6, %edi
	callq	square@PLT
	movl	%eax, %r14d
	movl	%ebx, %edi
	callq	construct_dump_int@PLT
	movl	%ebp, %edi
	callq	construct_dump_int@PLT
	movl	%r14d, %edi
	callq	construct_dump_int@PLT
	movl	%r14d, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
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
	.section	".note.GNU-stack","",@progbits
