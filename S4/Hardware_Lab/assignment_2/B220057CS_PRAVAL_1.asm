.data
	string1: .asciiz "Enter a number\n"
	odd_string: .asciiz "The number is Odd\n"
	even_string: .asciiz "The number is even\n"

.text
	main:
		li $v0, 4
		la $a0, string1
		syscall
		
		li $v0, 5
		syscall
		
		move $t0, $v0
		
		andi $t1, $t0, 1
		beq $t1, $zero, even
		j odd

	even:
		li $v0, 4
		la $a0, even_string
		syscall
		j end
		
	odd:
		li $v0, 4
		la $a0, odd_string
		syscall
	end: