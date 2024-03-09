.data
	arr: .word 2, 3, 4, 5, 6	
	arr2: .space 40
	
.text
	li $t0, 0
	la $s0, arr2
	li $s1, 5
		
	for:
		beq $t0, $s1, exit
		sll $t1, $t0, 2
		add $t1, $t1, $s0
		
		li $v0, 5
		syscall
		
		move $t2, $v0
		
		sw $t2, 0($t1)
		addi $t0, $t0, 1
		j for
	exit:
		la $s0, arr2
		li $t0, 0
	print:	
		beq $t0, $s1, end
		sll $t1, $t0, 2
		add $t1, $t1, $s0
		lw $t2, 0($t1) 
		li $v0, 1 
		move $a0, $t2 
		syscall

		li $a0, 32 
		li $v0, 11
		syscall
		addi $t0, $t0, 1
		j print
	end:
		li $v0,10 
		syscall