.data
	string1: .asciiz "Enter a string\n"
	input: .space 20
	max_size: .word 20
.text
	main:
		la $a0, string1
		li $v0, 4
		syscall
		
		li $v0, 8
		la $a0, input
		li $a1, 20
		syscall
		
		li $v0, 4
		syscall
	
		li $s1, 0	
	len:
		lb $t1, input($s1)
		beq $t1, 10, start
		addi $s1, $s1, 1
		j len
		
	start:
		li $v0, 1
		move $a0, $s1
		syscall
		
		
		
