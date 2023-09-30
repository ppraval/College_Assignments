#include <stdio.h>
#include <stdlib.h>

int main()
{
  int a ; 
  printf ("Input a number:");
  scanf ("%d", &a);
  if ((a%2) == 1)
  {
     printf ("It's an odd number.");
  }
  else 
  {  printf ("It's an even number.");
  }
  return 0;   
}