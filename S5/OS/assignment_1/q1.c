#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Praval Pattam
//B220057CS
int recursive(long int n) 
{
    if (n == 0) 
    {
        return 0;
    }
    char* s = (char*)malloc(sizeof(char) * 10);
    strcpy(s, "CDPCS-310");

    printf("\n%s\n\n", s);
    printf("Address allocated for s: %p\n", (void *)s);
    return recursive(n - 1);
}

int main() 
{
    long int n;
    printf("Enter the value of n: ");
    scanf("%ld", &n);
    printf("Address for Main function: %lx\n", (long unsigned int)&main);
    printf("Address for Recursive function: %lx\n", (long unsigned int)&recursive);
    printf("Address for n: %p\n", (void *)&n);
    recursive(n - 1);
    return 0;
}


// Objdump Output:
// The addresses of the main and recurse functions will be defined in the output of objdump -sRrd a.out as they are part of the executable’s symbol table.

// Stack Growth Direction:
// The stack typically grows downwards, meaning each new frame is allocated at a lower memory address than the previous one.

// Stack Frame Size:
// The stack frame size for each recursive call includes the space for the local variables and the return address. This can vary depending on the system and compiler, but typically it includes the size of the long int parameter and the return address.
// 48 bits

// Heap Location and Growth Direction:
// The heap is located in a different segment of memory than the stack and typically grows upwards, meaning each new allocation is at a higher memory address than the previous one.

// Contiguity of malloced Memory Areas:
// The memory areas allocated by malloc are not guaranteed to be contiguous. There can be extra space between their addresses due to the way the heap manager handles memory allocation and fragmentation.
