.data

msg1: .asciiz "Enter first number: "
msg2: .asciiz "Enter second number: "
msg3: .asciiz "Second number is less than First, exchanging and finding\n"
msg4: .asciiz "no number in between the range!\n"
msg5: .asciiz "negative range!\n"
endline: .asciiz " "

.text
.globl main

main:
    # put mssge 1
    	li $v0, 4
	la $a0, msg1
    	syscall
	# take n1
    	li $v0, 5
	syscall
    	move $t1, $v0
    # put mssge 2
    	li $v0, 4
    	la $a0, msg2
    	syscall
    # take n2
    	li $v0, 5
    	syscall
    	move $t2, $v0
    # if n1 == n2
    	bne $t1, $t2, continue1
    	li $v0, 4
    	la $a0, msg4
    	syscall
    	j exit

continue1:

    	blt $t1,$t2, continue2
   	li $v0, 4
    	la $a0, msg3
    	syscall

    	move $t4, $t1
    	move $t1, $t2
    	move $t2, $t4

continue2:

    	bgt $zero,$t1, negRange
    	j continue3

negRange:
        li $v0, 4
        la $a0, msg5
        syscall

        j exit

continue3:
    	addi $t1, $t1, 1

    # n and n+1 handle
    	beq $t1, $t2, noRange
    	j loop

noRange:
        li $v0, 4
        la $a0, msg4
        syscall
        j exit
    # for loop for printing primes
loop:       
        # put num in $a0
        #checkPrime called with jal 
        move $a0, $t1
        jal checkPrime
        #if $v0 is yes print else dont
        move $t8, $v0
        beq $t8, $zero, continue

        li $v0, 1
        move $a0, $t1
        syscall
        li $v0, 4
        la $a0, endline
        syscall
continue:
        #update n1
        addi $t1, $t1, 1
        #loop till _i < n2
        beq $t1, $t2, end_loop
        j loop
end_loop:

exit:
    	li $v0, 10  
    	syscall 

# function for checking a number prime
# a0 gets number, v0 gets the return yes/no
# without stack

checkPrime:
    	li $t0, 2
    # loop
    	li $t6, 1
    	beq $a0, $t6, not_prime 
    	loopCheck:
        rem $t3, $a0, $t0
        beq $t3, $zero, not_prime
        addi $t0, $t0, 1
        beq $t0, $a0, end_loop_yes
        j loopCheck
    # put yes/no in $v0
not_prime:
        li $v0, 0
        jr $ra
end_loop_yes:
        li $v0, 1
        jr $ra
