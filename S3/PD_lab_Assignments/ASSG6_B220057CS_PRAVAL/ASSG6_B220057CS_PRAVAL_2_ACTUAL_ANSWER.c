#include<stdio.h>

//this is supposed to be a stack implementation but couldn't make it work

int main()
{
    int size, next;
    scanf("%d", &size);
    int arr[size], newarr[size];
    for(int i = 0; i < size; i++)
    {
        scanf("%d", &arr[i]);
    }
    for(int i = 0; i < size; i++)
    {
        next = -1;
        for(int j = i + 1; j < size; j++)
        {
            if (arr[i] < arr[j])
            {
                next = arr[j];
                break;
            }
        }
        newarr[i] = next;
    }

    for(int i = 0; i < size; i++)
    {
        if(i == 0)
        {
            printf("%d", newarr[i]);
        }
        else
        {
            printf(" %d", newarr[i]);
        }
    } 
    return 1;
}