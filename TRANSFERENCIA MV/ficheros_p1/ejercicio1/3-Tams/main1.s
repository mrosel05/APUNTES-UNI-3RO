	.file	"main1.c"
	.text
	.globl	a
	.data
	.type	a, @object
	.size	a, 1
a:
	.byte	122
	.globl	b
	.align 4
	.type	b, @object
	.size	b, 4
b:
	.long	41
	.section	.rodata
.LC0:
	.string	"a = %d a = %c \n"
.LC1:
	.string	"a = %d a = %c b=%d  b=%c\n"
.LC2:
	.string	"Size of int: %lu\n"
.LC3:
	.string	"Size of char: %lu\n"
.LC4:
	.string	"Size of float: %lu\n"
.LC5:
	.string	"Size of double: %lu\n"
.LC6:
	.string	"Size of long: %lu\n"
.LC7:
	.string	"Size of short: %lu\n"
.LC8:
	.string	"Size of void*: %lu\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movzbl	a(%rip), %eax
	movsbl	%al, %edx
	movzbl	a(%rip), %eax
	movsbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movzbl	a(%rip), %eax
	addl	$6, %eax
	movb	%al, a(%rip)
	movl	b(%rip), %esi
	movl	b(%rip), %ecx
	movzbl	a(%rip), %eax
	movsbl	%al, %edx
	movzbl	a(%rip), %eax
	movsbl	%al, %eax
	movl	%esi, %r8d
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$4, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, %esi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$4, %esi
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$8, %esi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$8, %esi
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$2, %esi
	leaq	.LC7(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$8, %esi
	leaq	.LC8(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04.3) 11.4.0"
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
