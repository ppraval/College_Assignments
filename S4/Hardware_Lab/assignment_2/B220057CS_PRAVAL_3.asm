.data
	string1: .asciiz "Enter a number\n"
	yes_palindrome: .asciiz  "Yes it is a Palindrome"
	no_palindrome: .asciiz "No it is not a palindrome"
	
.text
	main:
		la $a0, string1
		li $v0, 4
		syscall
		
		li $v0, 5
		syscall
		
		move $t0, $v0
		move $t1, $zero
		move $t2, $t0
		
	palindrome_loop:
		divu $t2, $t2, 10
		mfhi $t3
		mul $t1, $t1, 10
		add $t1, $t1, $t3
		
		bnez $t2, palindrome_loop
		
		beq $t1, $t0, isPalindrome
		
		la $a0, no_palindrome
		li $v0, 4
		syscall
		j exit
		
	isPalindrome:
		la $a0, yes_palindrome
		li $v0, 4
		syscall
		j exit
		
	exit:	
		
		