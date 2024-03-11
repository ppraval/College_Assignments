.data
array:  .space 40  
prompt: .asciiz "Enter a number: " 
spacee: .asciiz " "
output: .asciiz "The numbers are: "
.text

main:
 li $t1,10         #load 10 into $t1
 la $a1,array      #load a pointer to array into $a1

loop:
 addi $t1,$t1,-1   #subtract 1 from $t1, save to $t1
 li $v0,4          #load 4 into $v0 (print string)
 la $a0,prompt     #load prompt text into $a
 syscall           #display prompt
 li $v0,5          #load 5 into $v0 (read integer)
 syscall           #prompt for input
 sw $v0,0($a1)     #store input int to array
 addi $a1,$a1,4    #add 4 to $a1, save to $a1
 bnez $t1,loop     #if $t1 isn't zero,goto loop
 li $t1,9          #if $t1 is zero, load 9 into $t1
 li $t2,9          #and load 9 into $t2
 la $a1,array      #load array pointer into $a1

loop1:
 beqz $t2,here     #if $t2 is zero, goto here
 addi $t2,$t2,-1   #subtract 1 from $t2, save to $t2
 lw $t5,0($a1)     #load an input int into $t5
 lw $t6,4($a1)     #load the next one into $t6
 addi $a1,$a1,4    #add 4 to $a1, save to $a1
 ble $t5,$t6,loop1 #if $t5 <= $t6, goto loop1
 sw $t5,0($a1)     #else, store $t5 in $a1
 sw $t6,-4($a1)     #and store $t6 in $a1-4 (swapping them)
 bnez $t2,loop1    #if $t2 is not zero, to go loop1

here:
 la $a1,array      #load array into $a1
 addi $t1,$t1,-1   #subtract 1 from $t1, save to $t1
 add $t2,$t2,$t1   #add $t2 to $t1, save to $t2
 bnez $t1,loop1    #if $t1 isn't zero, goto loop1
 li $v0,4          #load 4 into $v0 (print string)
 la $a0,output     #load 'the numbers are' into $a0
 syscall           #display message to screen
 la $a1,array      #load array pointer into $a1
 li $t1,10         #load 10 into $t1

loop2:
 li $v0,1          #load 1 into $v0 (print int)
 lw $a0,0($a1)     #load $a1 into $a0
 syscall           #print first number to screen
 li $v0,4          #load 4 into $v1 (print string)
 la $a0,spacee     #load ' ' into $a0
 syscall           #print ' ' to screen
 addi $a1,$a1,4    #add 4 to $a1, save to $a1
 addi $t1,$t1,-1   #subtract 1 from $t1, save to $t1
 bnez $t1,loop2    #if $t1 isn't zero, goto loop2

 li $v0,10              #exit

 syscall