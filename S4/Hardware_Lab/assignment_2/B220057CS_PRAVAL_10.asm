.data
prompt: .asciiz "Enter a signed integer A: "
result: .asciiz "The result of adding A and B is: "

.text
main:
    li $v0, 4
    la $a0, prompt
    syscall

    li $v0, 5
    syscall

    move $t0, $v0

    not $t1, $t0
    addiu $t1, $t1, 1
    add $t2, $t0, $t1

    li $v0, 4
    la $a0, result
    syscall

    move $a0, $t2
    li $v0, 1
    syscall

    li $v0, 10
    syscall
