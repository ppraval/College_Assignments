#include <stdio.h>
int main()
{
    char L;
    printf ("pls enter a random capital letter :");
    scanf ("%c", &L);
    if ( L > 64 && L < 91)
    {
        L = L + 32;
        printf ("The entered number in lower case is %c", L);
    }
    else
    {
        printf ("I SAID CAPITAL LETTER U SHIXXXX");
    }
    return 0;
}