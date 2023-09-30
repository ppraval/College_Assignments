#include <stdio.h>
#include <stdlib.h>

int main ()
{
int a;
printf ("Insert your 2/3 digit number: ");
scanf ("%d", &a);
    if ((a / 100) == 0)
    {
       if ((a / 10) == 0) {printf ("DO NOT ENTER A ONE DIGIT NUMBER YOU IDOT");}
       else {printf ("Two digit number");}
    }
    else if (a / 1000 == 0){printf ("three digit number");}
    else {printf ("Pls enter a two/three digit number you imbecile");}
return 0;

//re write this code

}
