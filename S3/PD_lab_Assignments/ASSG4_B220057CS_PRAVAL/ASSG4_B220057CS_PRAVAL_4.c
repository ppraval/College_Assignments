#include<stdio.h>


int main()
{
    int n, min, pos, temp, energy, e;
    scanf("%d", &n);
    int arr[10000];
    for(int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }
    scanf("%d", &e);
    energy = 0;
    for (int i = 0; i < n; i++)
    {   
        min = arr[i];
        pos = i;
        for (int j = i + 1; j < n; j++)
        {
            if (i % 2 == 0)
            {
                if (arr[j] < min)
                {
                    min = arr[j];
                    pos = j;
                }
            }
            if (i % 2 == 1)
            {
                if (arr[j] > min)
                {
                    min = arr[j];
                    pos = j;
                }
            }
        }
        if (pos != i)
        {
            int temp = arr[i];
            arr[i] = arr[pos];
            arr[pos] = temp;
            energy++;
        }
        if (energy >= e)
        {
            break;
        }
    } 
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