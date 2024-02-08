#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int hash(int val, int size)
{
    return val % size;
}

void linearProbing(int arr[], int size, int n)
{
    int hash_table[100];
    for(int i = 0; i < size; i++)
        hash_table[i] =  -1;
    int collisions = 0;
    for(int i = 0; i < 6; i++)
    {
        int index = hash(arr[i], size);
        int j = index;
        if(hash_table[index] == -1)
        {
            hash_table[index] = arr[i];
        }   
        else
        {
            int k = 1;
            while(hash_table[j] != -1 && j < size)
            {   
                j = (index + k) % size;
                collisions++;
                if(j == index)
                {
                    printf("-1\n"); 
                    return; 
                }
                k++;
            }
            hash_table[j] = arr[i];
        }
        printf("%d ", j);
    }
    printf("\n%d\n", collisions);
}

void quadraticProbing(int arr[], int size, int n)
{
    int hash_table[100];
    // int n = sizeof(arr) / sizeof(arr[0]);
    for(int i = 0; i < size; i++)
        hash_table[i] =  -1;
    int collisions = 0;
    for(int i = 0; i < 6; i++)
    {
        int index = hash(arr[i], size);
        int j = index;
        if(hash_table[index] == -1)
        {
            hash_table[index] = arr[i];
        }   
        else
        {
            int k = 1;
            while(hash_table[j] != -1 && j < size)
            {   
                j = (index + k*k) % size;
                collisions++;
                if(j == index)
                {
                    printf("-1\n"); 
                    return; 
                }
                k++;
            }
            hash_table[j] = arr[i];
        }
        printf("%d ", j);
    }
    printf("\n%d\n", collisions);
}

int maxPrime(int n) 
{
    int i, j, flag;
    for (i = n - 1; i >= 2; i--) 
    {
        flag = 1;
        for (j = 2; j < i; j++) 
        {
            if (i % j == 0) 
            {
                flag = 0;
                break;
            }
        }
        if (flag == 1) 
        {
            return i;
        }
    }
    return -1;
}

void doubleHashing(int arr[], int size, int n)
{
    int hash_table[100];
    // int n = sizeof(arr) / sizeof(arr[0]);
    int r = maxPrime(size);
    for(int i = 0; i < size; i++)
        hash_table[i] =  -1;
    int collisions = 0;
    for(int i = 0; i < 6; i++)
    {
        int index_1 = hash(arr[i], size);
        int index_2 = r - hash(arr[i], r);
        int j = index_1;
        if(hash_table[index_1] == -1)
        {
            hash_table[index_1] = arr[i];
        }   
        else
        {
            int k = 1;
            while(hash_table[j] != -1 && j < size)
            {   
                j = (index_1 + k*(index_2)) % size;
                collisions++;
                if(j == index_1)
                {
                    printf("-1\n"); 
                    return; 
                }
                k++;
            }
            hash_table[j] = arr[i];
        }
        printf("%d ", j);
    }
    printf("\n%d\n", collisions);
}

int main()
{
    int size, j = 0;
    int arr[100];
    char str[1000];
    scanf("%d\n", &size);
    fgets(str, 100, stdin);
    int val = 0;
    for(int i = 0; i < strlen(str); i++)
    {
        if(str[i] == ' ' || i == strlen(str) - 1)
        {
            arr[j++] = val;
            val = 0;
            continue;
        }
        val = val * 10 + str[i] - '0';
    }
    linearProbing(arr, size, j);
    quadraticProbing(arr, size, j);
    doubleHashing(arr, size, j);
    return 0;
}