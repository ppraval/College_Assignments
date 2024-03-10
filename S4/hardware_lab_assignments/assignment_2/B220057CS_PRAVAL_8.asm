.data
array: .word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
key: .word 109
size: .word 10
found_msg: .asciiz "Key found at index: "
not_found_msg: .asciiz "Key not found."

.text
main:
    la $a0, array 
    lw $a1, key 
    lw $a2, size
    li $t0, 0 

linear_search:
    beq $t0, $a2, not_found 
    lw $t1, 0($a0) 
    beq $t1, $a1, found
    addi $a0, $a0, 4 
    addi $t0, $t0, 1 
    j linear_search 

found:
    la $a0, found_msg
    li $v0, 4 
    syscall
    move $a0, $t0
    li $v0, 1 
    syscall
    j exit

not_found:
    la $a0, not_found_msg 
    li $v0, 4 
    syscall

exit:
