#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, k;
    int arr[1000];
    scanf("%d %d", &n, &k);
    for(int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }
    int* table = (int*)malloc(sizeof(int) * 1000);
    for(int i = 0; i < 1000; i++) 
    {
        table[i] = 0;
    }
    int count = 0;
    for(int i = 0; i < k; i++) 
    {
        if (table[arr[i]] == 0) 
        {
            count++;
        }
        table[arr[i]]++;
    }
    printf("%d ", count);
    for(int i = k; i < n; i++) 
    {
        if(table[arr[i - k]] == 1) 
        {
            count--;
        }
        table[arr[i - k]]--;
        if(table[arr[i]] == 0) 
        {
            count++;
        }
        table[arr[i]]++;
        printf("%d ", count);
    }
    return 0;
}