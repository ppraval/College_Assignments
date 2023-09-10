#include<stdio.h>

int main()
{
    int N, X, D;
    scanf("%d", &N);
    float arr[1000];
    for (int k = 0; k < N; k++)
    {
        scanf("%f", &arr[k]);
    }
    scanf("%d", &X);
    scanf("%d", &D);
    int i, j;
    float key;
    for (i = 1; i < N - X + D; i++)
    {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }

    for (int u = 0; u < N; u++)
    {
      if( u == 0)
      {
        printf("%.2f ", arr[u]);
      }
      else
      {
        printf(" %.2f", arr[u]);
      }
    }
  return 1;
}