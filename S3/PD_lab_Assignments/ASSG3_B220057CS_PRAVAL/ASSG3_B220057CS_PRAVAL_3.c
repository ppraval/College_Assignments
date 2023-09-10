#include<stdio.h>

int main() 
{
  int i, j, temp, n;
  scanf("%d", &n);
  int arr[1000];
  for (int h = 0; h < n; h++)
  {
    scanf("%d", &arr[h]);
  }
  for(i=0; i<n; i++)
  {
    for(j=i+1; j<n; j++)
    {
      if(arr[i]< arr[j])
      {
      temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
      }
    }
  }
  
  for (int k = 0; k < n; k++) 
  {
    if (k == 0)
    {
        printf("%d", arr[k]);
    }
    else
    {
        printf(" %d", arr[k]);
    }
  }
  return 1;
}
