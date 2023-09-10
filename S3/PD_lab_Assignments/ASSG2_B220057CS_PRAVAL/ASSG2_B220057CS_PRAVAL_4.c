#include<stdio.h>
#include<stdlib.h>

int peak_value(int number, int array[])
{
    int low = 0, high, mid, peak;    
    high = number - 1;

    while( low < high ) 
    {
        mid = (low + high) / 2;
        
        if(array[mid] > array[high])
        {
            low = mid + 1;
        }
        else 
        {
            high = mid;
        }
    }
    if(low == 0)
        return number - 1;
    else
        return low - 1;
}
int secret_number_search(int low, int high, int secret_number, int array[])
{
    int mid;
        while (low <= high)
        {
            mid = low + (high - low) / 2;
            if (array[mid] == secret_number)
            {
                return mid;
            }
            if (array[mid] < secret_number)
            {
                low = mid + 1;
            }
            else
            {    
                high = mid - 1;
            }
        }
    return -1;
}

int main()
{
    int number, secret_number, peak, low, high;
    scanf("%d", &number);
    int *array = malloc(sizeof(int) * number);
    for (int i = 0; i < number; i++)
    {
        scanf("%d", &array[i]);
    }
    scanf("%d", &secret_number);
    peak = peak_value(number, array);
    if (secret_number >= array[0] && secret_number <= array[peak])
    {   
        low = 0;
        high = peak;
    }
    else 
    {
        low = peak + 1;
        high = number - 1;
    }
    printf("%d", secret_number_search(low, high, secret_number, array));
    free(array);
    return 1;
}