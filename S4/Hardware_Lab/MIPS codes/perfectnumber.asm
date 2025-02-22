.data
	string1: .asciiz "Enter a number\n"
	yes_number: .asciiz "It is a perfect number"
	no_number: .asciiz "It is not a perfect number"
.text
main: 
	li $v0, 4        
    	la $a0, string1    
    	syscall            
    	li $v0, 5         
    	syscall             

	move $s0, $v0      
    	li $s1, 0          
    	li $s2, 1     

loop:  
	bgeu $s2, $s0, evaluate
	rem $t0, $s0, $s2   
	bne $t0, $0, add_break
	addu $s1, $s1, $s2
	
add_break:  
	addi $s2, $s2, 1    
    	j loop           

evaluate:
	beq $s1, $s0, ans
	li $v0, 4          
    	la $a0, no_number    
    	syscall
    	j exit

ans: 
    	li $v0, 4          
    	la $a0, yes_number    
    	syscall
    	j exit

exit:
	