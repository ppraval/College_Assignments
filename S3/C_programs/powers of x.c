#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int x;
    printf("Input Your number here: ");
    scanf ("%d", &x);
    printf ("Square of x is %.0lf, Fourth power of x is %.0lf, Sixth power of x is %.0lf, Eight power of x is %.0lf", pow(x, 2), pow(x, 4), pow(x, 6), pow(x, 8));
    return 0;

}