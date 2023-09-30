#include <stdio.h>
int main()
{
    int num;
    float hun;
    float ten;
    float one;
    printf ("Enter a thee digit number ");
    scanf ("%d", &num);
    hun = (int)(num / 100);
    ten = (int)((num - hun*100)/10);
    one = (int)(num - hun*100 - ten*10);
    printf("The sum of digits is = %.0f\n", hun + ten + one);


    int units, tens, huns;
    units = num % 10;
    tens = (num / 10) % 10;
    huns = num/100;
    printf("the sum of digits is also = %d", huns+tens+units);
    return 0;
    
}