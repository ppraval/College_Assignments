#include <stdio.h>
int main()
{
    int num;
    int units, tens, hundreds;
    printf("Enter a three digit number :");
    scanf("%d", &num);
    units = num % 10;
    tens = (num / 10) % 10;
    hundreds = num / 100;

    printf ("the reverse number is %d\n", units*100 + tens*10 + hundreds);
    
    int reversenum;
    reversenum = units;
    reversenum = reversenum*10 + tens;
    reversenum = reversenum*10 + hundreds;
    printf (" the reversenum is also %d\n", reversenum);
     

    printf("The reverse number is %d%d%d\n", (num % 10), ((num / 10 ) % 10), (num / 100));
    return 0;
     
    }