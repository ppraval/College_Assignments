#include <stdio.h>

int main()
{
    float data;
    int step;
    float frac;
    printf ("input a decimal number : ");
    scanf ("%f", &data);
    step = data;
    frac = data - step;
    printf("The fractional part of the given number is = %.2f\n", frac);
    
    printf("the fractional part of the number is also = %.2f\n", data - (int)data);
    return 0;

}