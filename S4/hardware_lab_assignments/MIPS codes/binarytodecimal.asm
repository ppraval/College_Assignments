.data
  msg1:
    .asciiz "Enter a binary number "
  empty:
    .space 16
  sum:
    .space 16 
  sumMsg:
    .asciiz "Decimal Equivalent: "
.text
.globl main
main:

getNum:
	li $v0,4        
	la $a0, msg1      
	syscall

	la $a0, empty
	li $a1, 32             
	li $v0, 8               
	syscall

	li $t4, 0         
	
startConvert:
	la $t1, empty
	li $t9, 16             

firstByte:
  	lb $a0, ($t1)      
  	blt $a0, 48, printSum
  	addi $t1, $t1, 1      
  	subi $a0, $a0, 48      
	subi $t9, $t9, 1        
 	beq $a0, 0, isZero
	beq $a0, 1, isOne
	j convert

isZero:
   	j firstByte

isOne:                   
   	li $t8, 1              
   	sllv $t5, $t8, $t9    
   	add $t4, $t4, $t5      

	j firstByte

convert:

printSum:
   	srlv $t4, $t4, $t9
   	la $a0, sumMsg
   	li $v0, 4
   	syscall

 	move $a0, $t4     
	li $v0, 1     
	syscall

exit:
   	li $v0, 10
	syscall
