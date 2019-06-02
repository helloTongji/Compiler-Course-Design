	.text
		ori	$fp	$sp	0
		li	$t9	0x7fffeffc	#global stack bottom
		li	$t8	0x10010000	#save word
		li	$t0	0	#a
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#a
		sw	$t0	($sp)
		subi	$sp	$sp	4
		subi	$sp	$sp	20
		j	__main
		li	$t0	0	#a
		sw	$t0	($sp)
		subi	$sp	$sp	4
		j	__main
program:
		#Save Register
		sw	$fp	($sp)
		add	$fp	$sp	$0
		subi	$sp	$sp	4
		sw	$ra	($sp)
		subi	$sp	$sp	4
		#Save Register Done!
		li	$t0	0	#$_0
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#$_1
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#$_2
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#$_3
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#$_4
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#program
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#program
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#program
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0
		sw	$t0	-28($fp)
		lw	$t0	8($fp)
		lw	$t1	4($fp)
		add	$t0	$t0	$t1
		sw	$t0	-8($fp)
		lw	$t0	12($fp)
		lw	$t1	-8($fp)
		slt	$t0	$t1	$t0
		bne	$t0	1	_LABEL_0//ÅÐ¶Ïa>(b+c)
		lw	$t0	8($fp)
		lw	$t1	4($fp)
		mul	$t0	$t0	$t1
		sw	$t0	-12($fp)
		lw	$t0	-12($fp)
		li	$t1	1
		add	$t0	$t0	$t1
		sw	$t0	-16($fp)
		lw	$t0	12($fp)
		lw	$t1	-16($fp)
		add	$t0	$t0	$t1
		sw	$t0	-20($fp)
		lw	$t0	-20($fp)
		sw	$t0	-32($fp)
		j	_LABEL_1
_LABEL_0://a<=(b+c)
		lw	$t0	12($fp)
		sw	$t0	-32($fp)
_LABEL_1:
_LABEL_2://i<=100
		lw	$t0	-28($fp)
		li	$t1	100
		slt	$t0	$t1	$t0
		li	$t1	1
		sub	$t0	$t1	$t0
		bne	$t0	1	_LABEL_3
		lw	$t0	-32($fp)
		li	$t1	2
		mul	$t0	$t0	$t1
		sw	$t0	-24($fp)
		lw	$t0	-24($fp)
		sw	$t0	-28($fp)
		lw	$t0	-28($fp)
		sw	$t0	-32($fp)
		j	_LABEL_2
_LABEL_3:
		lw	$v0	-28($fp)
		j	__FEND_LAB_1
__FEND_LAB_1:
		lw	$ra	-4($fp)
		add	$sp	$fp	$0
		lw	$fp	($fp)
		jr	$ra	
demo:
		#Save Register
		sw	$fp	($sp)
		add	$fp	$sp	$0
		subi	$sp	$sp	4
		sw	$ra	($sp)
		subi	$sp	$sp	4
		#Save Register Done!
		li	$t0	0	#$_0
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#$_1
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#demo
		sw	$t0	($sp)
		subi	$sp	$sp	4
		lw	$t0	4($fp)
		li	$t1	2
		add	$t0	$t0	$t1
		sw	$t0	-8($fp)
		lw	$t0	-8($fp)
		sw	$t0	4($fp)
		lw	$t0	4($fp)
		li	$t1	2
		mul	$t0	$t0	$t1
		sw	$t0	-12($fp)
		lw	$t0	-12($fp)
		sw	$t0	4($fp)
		li	$t0	0
		sw	$t0	-16($fp)
		lw	$v0	4($fp)
		j	__FEND_LAB_2
__FEND_LAB_2:
		lw	$ra	-4($fp)
		add	$sp	$fp	$0
		lw	$fp	($fp)
		jr	$ra	
__main:
		#Save Register
		sw	$fp	($sp)
		add	$fp	$sp	$0
		subi	$sp	$sp	4
		sw	$ra	($sp)
		subi	$sp	$sp	4
		#Save Register Done!
		li	$t0	0	#$_0
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#$_1
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#main
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#main
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0	#main
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	3
		sw	$t0	-16($fp)
		li	$t0	4
		sw	$t0	-20($fp)
		li	$t0	2
		sw	$t0	-24($fp)
		lw	$t0	-24($fp)
		sw	$t0	($sp)
		subi	$sp	$sp	4
		jal	demo
		nop
		sw	$v0	-8($fp)
		lw	$t0	-16($fp)
		sw	$t0	($sp)
		subi	$sp	$sp	4
		lw	$t0	-20($fp)
		sw	$t0	($sp)
		subi	$sp	$sp	4
		lw	$t0	-8($fp)
		sw	$t0	($sp)
		subi	$sp	$sp	4
		jal	program
		nop
		sw	$v0	-12($fp)
		lw	$t0	-12($fp)
		sw	$t0	-16($fp)
		j	__FEND_LAB_3
__FEND_LAB_3:
		lw	$ra	-4($fp)
		add	$sp	$fp	$0
		lw	$fp	($fp)
		li	$v0	10
		syscall
