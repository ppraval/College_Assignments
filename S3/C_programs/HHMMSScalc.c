#include <stdio.h>

int main()
{
    int h,m,s;
    printf ("Please enter a random number of seconds :\n");
    scanf ("%d", &s);
    h = (s / 3600);
    m = (s - (h*3600)) / 60;
    s = (s - h*3600 - m*60 );
    
    if (h < 10) {printf ("0");}
    printf ("%d Hours ", h);
    if (m < 10) {printf ("0");}
    printf ("%d Minutes ", m);
    if (s < 10) {printf ("0");}
    printf ("%d Seonds ", s);
    return 0;
}