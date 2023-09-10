#include<stdio.h>

int main()
{
    int i, j, key, n;
    scanf("%d", &n);
    int arr[1000];
    for ( int h = 0; h < n; h++)
    {
        scanf("%d", &arr[h]);
    }
    for (i = 1; i < n; i++)
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