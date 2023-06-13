	.file	"01_var_defn2.c"
	.option nopic
	.text
	.globl	a
	.section	.sdata,"aw"
	.align	2
	.type	a, @object
	.size	a, 4
a:
	.word	3
	.globl	b
	.align	2
	.type	b, @object
	.size	b, 4
b:
	.word	5
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32

	li	a5,5
	sw	a5,-20(s0)
	lui	a5,%hi(b)
	lw	a4,%lo(b)(a5)
	lw	a5,-20(s0)
	add	a5,a4,a5
	mv	a0,a5
	
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 9.2.0"
	.section	.note.GNU-stack,"",@progbits
