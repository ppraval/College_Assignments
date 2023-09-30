#include <stdio.h>

int main ()
{
float a;
float d;
float n;
float Sn;
printf("The first term = ");
scanf("%f", &a);
printf("the commoon difference = ");
scanf ("%f", &d);
printf ("the value of n = ");
scanf ("%f", &n);
printf("the sum of n terms is =%f\n", (n/2)*(2*a + (n-1)*d));
return  0;

}