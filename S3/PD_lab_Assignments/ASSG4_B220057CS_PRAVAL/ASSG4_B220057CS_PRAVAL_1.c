#include<stdio.h>

int insertion_sort(float arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        float key = arr[i];
        int j = i - 1;
        while(j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    return 0;
}

int main()
{
    int n;
    scanf("%d", &n);
    float arr[100], bucket[10][100], min, max, range;
    int bucket_count[10] = {0};

    for (int i = 0; i < n; i++)
    {
        scanf("%f", &arr[i]);
    }
    max = arr[0];
    min = arr[0];
    for (int i = 0; i < n; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }

    range = (max - min + 1) / 10;
    for (int i = 0; i < n; i++)
    {
        int index = (arr[i] - min) / range;
        bucket[index][bucket_count[index]] = arr[i];
        bucket_count[index]++;
    }
    for (int i = 0; i < 10; i++) 
    {
        insertion_sort(bucket[i], bucket_count[i]);
    }
    int index = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < bucket_count[i]; j++)
        {
            arr[index] = bucket[i][j];
            index++;
        }    
    }
    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            printf("%.2f", arr[i]);
        }
        else
        {
            printf(" %.2f", arr[i]);
        }
    }
    return 1;
}