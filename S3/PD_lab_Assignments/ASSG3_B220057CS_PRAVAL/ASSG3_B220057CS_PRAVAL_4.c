#include<stdio.h>

int main() 
{
  int i, j, key, N, K;
  scanf("%d", &N);
  scanf("%d", &K);
  int arr[1000];
  for (int h = 0; h < N; h++)
  {
    scanf("%d", &arr[h]);
  }
  for (i = 1; i < N; i++)
  {
    key = arr[i];
    j = i - 1;
    while (j >= 0 && arr[j] < key)
    {
        arr[j + 1] = arr[j];
        j = j - 1;
    }
    arr[j + 1] = key;
  }
  
  for (int k = 0; k < K; k++) 
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
