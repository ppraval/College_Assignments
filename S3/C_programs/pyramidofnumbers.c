#include <stdio.h>
#include <stdlib.h>
 int main ()
{
  int n, count = 1;
  printf ("please enter a number");
  scanf("%d", &n);
  for (int i = 1; i <= n; i++)
  {
    for (int a = 0; a < n - i; a++)
    {
      printf (" ");
    }
    for (int b = 0; b < i; b++)
    {
      printf("%d ", count++);
    }
    printf("\n");
  }
  return 0;
}