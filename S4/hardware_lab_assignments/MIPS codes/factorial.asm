.data
	string1: .asciiz "Enter a number\n"
	ans_msg: .asciiz "The factorial is = "
	
.text
	main:
		li $v0, 4
		la $a0, string1
		syscall
		
		li $v0, 5
		syscall
		move $t0, $v0
			
		li $t1, 1
		li $t2, 1
		
	factorial_loop:
		mul $t2, $t2, $t1
		beq $t1, $t0, answer
		addi $t1, $t1, 1
		j factorial_loop
		
	answer:
		la $a0, ans_msg
		li $v0, 4
		syscall
		
		move $a0, $t2
		li $v0, 1
		syscall	
	