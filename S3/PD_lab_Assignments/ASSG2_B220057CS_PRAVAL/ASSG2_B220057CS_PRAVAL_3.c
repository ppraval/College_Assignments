#include<stdio.h>
#include<stdlib.h>

int main()
{
    int number, low = 0, high, mid, peak;
    scanf("%d", &number);
    int *array = malloc(sizeof(int) * number);
    for (int i = 0; i < number; i++)
    {
        scanf("%d", &array[i]);
    }
    
    high = number - 1;

    while( low <= high ) 
    {
        mid = (low + high) / 2;
        if ((mid == 0 || array[mid] > array[mid - 1]) && (mid == number - 1 || array[mid] > array[mid + 1]))
        {
            peak = array[mid];
            break;
        }
        else if (mid < number - 1 && array[mid] < array[mid + 1])
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    printf("%d\n", peak);
    free(array);
    return 1;
}
