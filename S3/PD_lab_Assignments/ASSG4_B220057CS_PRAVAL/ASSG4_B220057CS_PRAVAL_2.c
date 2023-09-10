#include<stdio.h>

int max_element(int arr[], int n) 
{
    int max = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > max)
        {
            max = arr[i];
        }
    return max;
}

int counting_sort(int arr[], int n, int place) 
{
    int final_arr[n];
    int count[10] = {0};
    for (int i = 0; i < n; i++)
    {
        count[(arr[i] / place) % 10]++;
    }

    for (int i = 1; i < 10; i++)
    {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) 
    {
        final_arr[count[(arr[i] / place) % 10] - 1] = arr[i];
        count[(arr[i] / place) % 10]--;
    }

    for (int i = 0; i < n; i++)
    {
    arr[i] = final_arr[i];
    }
    return 0;
}

int radix_sort(int arr[], int n) 
{
    int max = max_element(arr, n);
    for (int place = 1; max / place > 0; place *= 10)
    {
        counting_sort(arr, n, place);
    }
    return 0;
}


int main() 
{
    int n;
    scanf("%d", &n);
    int arr[1000];
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }
    radix_sort(arr, n);
    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            printf("%d", arr[i]);
        }
        else
        {
            printf(" %d", arr[i]);
        }
    }
    return 1;
}