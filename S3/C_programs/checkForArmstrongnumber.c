#include <stdio.h>
#include <stdlib.h>

char main()
{
    int num = 407, sum, count, result, temp, exponent;

    for (count = 0; num !=0 ; count++, num = num/10)

    temp = num;
    while (temp>0)
    {
        result=temp%10;
        for (exponent=count; exponent>0; exponent--)
        {
            result = result*count;
        }
        sum = sum + result;
        temp = temp/10;
    }
    printf ("%d", sum);

return 'a';
}