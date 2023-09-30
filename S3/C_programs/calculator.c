#include <stdio.h>
#include <stdlib.h>

int main ()
{
    float a, b;
    char c;
    printf("Enter your operation: ");
    scanf ("%c", &c);
    printf ("enter two numbers:\n");
    scanf ("%f", &a);
    scanf ("%f", &b);
    switch (c)
    {
        case '+': printf ("Your output is: %f", a + b);
        break;
        case '-': printf ("Your output is: %f", a - b);
        break;
        case '*': printf ("Your output is: %f", a * b);
        break;
        case '/': if (b == 0) {printf ("You can't divide by zero idot");} else {printf ("Your output is: %f", a / b);}
        break;
        case '%': printf ("Your output is: %d", (int)a % (int)b);
        break;
    }
    return 0;
}