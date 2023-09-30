#include <stdio.h>

int main()
{
    float a = 87.5;
    float b = 32.1;
    float temp;
    printf ("a = %f\n", a);
    printf ("b = %f\n", b);
    temp = a;
    a = b;
    b = temp;
    printf ("new a = %f\n", a);
    printf ("new b = %f\n", b);
    return 0;
}
