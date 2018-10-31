	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	pushq	%rax
	.cfi_offset %rbx, -24
	movl	$-1, %ebx
	cmpl	$2, %edi
	jne	LBB0_2
## %bb.1:
	movq	8(%rsi), %rax
	movsbl	(%rax), %esi
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	testl	%esi, %esi
	sete	%r9b
	setne	%r8b
	xorl	%eax, %eax
	cmpl	$5, %esi
	seta	%al
	leal	-2(,%rax,4), %edx
	leal	(%rax,%rax,2), %ecx
	leal	(%rax,%rax,4), %eax
	addl	%r8d, %r8d
	orl	%eax, %r8d
	decl	%r8d
	shll	$2, %r9d
	leaq	L_.str(%rip), %rdi
	xorl	%ebx, %ebx
	xorl	%eax, %eax
	callq	_printf
LBB0_2:
	movl	%ebx, %eax
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%d, %d, %d, %d, %d"


.subsections_via_symbols
