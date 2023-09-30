#include <stdio.h>
#include <stdlib.h>

int main ()
{
    int a, b, c, max1, min1 ;
    printf ("Input three numbers \n ");
    scanf ("%d", &a);
    scanf ("%d", &b);
    scanf ("%d", &c);
    if (a > b) 
    {
        max1 = a; min1 = b;
    }
    else 
    {
        max1 = b; min1 = a;
    }
    if (max1 > c) 
    {
        if ( min1 > c)
        {
          printf ("The max number is %d and the minimum number is %d", max1, c);
        }
        else 
        {
            printf ("The max number is %d and the minimum number is %d", max1, min1);
        }
    }
    else 
    {
        printf ("The max number is %d and the minimun number is %d\n", c, min1);
    }

int max = a, min = b;
if (a < b)
{
   max = b; min = a; 
}
if (c > max)
{
    max = c;
}
if (min > c)
{
    min = c;
}
printf ("The maximun number is %d, The minimum number is %d", max, min);
}