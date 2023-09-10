#include<stdio.h>

int max_element(char arr[], int size)
{
    int max = arr[0];
    for (int i = 0; i < size; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}
int partition (char arr[], int first, int last)  
{  
    int i, pivot, temp;

    pivot = arr[last]; 
    i = (first - 1);  
  
    for (int j = first; j <= last - 1; j++)  
    {  
        if (arr[j] < pivot)  
        {  
            i++;  
            temp = arr[i];  
            arr[i] = arr[j];  
            arr[j] = temp;  
        }  
    }  
    temp = arr[i+1];  
    arr[i+1] = arr[last];  
    arr[last] = temp;  
    return (i + 1);  
}  

int quick_sort(char arr[], int first, int last)
{  
    int n;
    if (first < last)  
    {  
        n = partition(arr, first, last);  
        quick_sort(arr, first, n - 1);  
        quick_sort(arr, n + 1, last);  
    }  
    return 1;
}  

int main()
{
    int n;
    scanf("%d", &n);
    char arr[1000];
    for (int i = 0; i < n; i++)
    {
        scanf(" %c", &arr[i]);
    }
    quick_sort(arr, 0, n - 1);
    for (int i = 0; i < n; i++)
    {
        if ( i == 0)
        {
            printf("%c", arr[n - i - 1]);
        }
        else
        {
            printf(" %c", arr[n - i - 1]);
        }
    }
    return 1;
}