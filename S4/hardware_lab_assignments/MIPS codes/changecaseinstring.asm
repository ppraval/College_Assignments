.data
    str: .space 256
    msg: .asciiz "Enter a string: "
.text
.globl main
main:

    	li $v0, 4
    	la $a0, msg
    	syscall

    	li $v0, 8
    	la $a0, str
    	li $a1, 256
    	syscall

    	la $a0, str                   
    loop:
        lb $t0, 0($a0)            
        beqz $t0, end             
        li $t1, 'A'               
        li $t2, 'Z'               
        li $t3, 'a'               
        li $t4, 'z'               

        blt $t0, $t1, not_upper
        bgt $t0, $t2, not_upper  
        addi $t0, $t0, 32         
        j store_char

    not_upper:
        blt $t0, $t3, not_lower
        bgt $t0, $t4, not_lower  
        subi $t0, $t0, 32         

    store_char:
        sb $t0, 0($a0)   
        addi $a0, $a0, 1  
        j loop             

    not_lower:
        addi $a0, $a0, 1    
        j loop               

    end:
        li $v0, 4
        la $a0, str
        syscall
