.data
	str: .asciiz "Please enter a number\n"
	no_no: .asciiz "It is not an armstrong number"
	yes_no: .asciiz "It is an armstrong number"
.text
	main:
		la $a0, str
		li $v0, 4
		syscall
		
		
		
		li $v0, 5
		syscall
		move $t0, $v0
		move $t2, $t0
		
		li $t1, 0
		
	loop:
		divu $t0, $t0, 10
		addi $t1, $t1, 1
		beq $t0, $zero, prep_sum
		j loop
	prep_sum:
		move $t3, $t2	
		li $t8, 0
	divide_and_sum:
		divu $t3, $t3, 10
		mfhi $t7
		li $t4, 0	
		li $t5, 1	
		move $t6, $t1
	power:
		mul $t5, $t5, $t7
		addi $t4, $t4, 1
		beq $t4, $t6, sum
		j power
	
	sum:
		add $t8, $t8, $t5
		
		beq $t3, $zero, evaluate
		j divide_and_sum
	evaluate:
		beq $t8, $t2, yes_armno
		la $a0, no_no
		li $v0, 4
		syscall
		j exit
	yes_armno:
		la $a0, yes_no
		li $v0, 4
		syscall
	exit:
