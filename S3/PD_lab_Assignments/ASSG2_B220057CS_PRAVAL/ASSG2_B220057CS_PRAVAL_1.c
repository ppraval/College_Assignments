#include<stdio.h>
#include<stdlib.h>

int main() 
{
    int number, required_num_k, mid, missing_num, high, low;
    scanf("%d", &number);
    int *array = malloc(sizeof(int) * number);

    for (int i = 0; i < number; i++)
    {
        scanf("%d", &array[i]);
    }
    scanf("%d", &required_num_k);
    high = number - 1;
    low = 0;
    if (required_num_k < array[low] || required_num_k > array[high] - number)
    {
        printf("%d", -1);
    }
    else
    {
        while(low <= high) 
        {
            mid = (low + high) / 2;
            missing_num = array[mid] - mid - 1;
            if(required_num_k <= missing_num) 
            {
                high = mid - 1;
            }
            else 
            {
                low = mid + 1;
            }
        }

        printf("%d", required_num_k + high + 1);
    }
    free(array);
    return 1;
}
