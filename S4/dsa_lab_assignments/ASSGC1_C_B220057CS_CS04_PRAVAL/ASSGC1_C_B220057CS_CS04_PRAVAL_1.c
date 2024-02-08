#include<stdio.h>
#include<stdlib.h>

struct pair
{
    int key;
    int val;
};

typedef struct pair* pair;

struct hash_table
{
    int max_size;
    int size;
    pair arr;
};

typedef struct hash_table* hash_table;

hash_table createHashTable(int max_size)
{
    hash_table new_table = malloc(sizeof(struct hash_table));
    new_table->max_size = max_size;
    new_table->size = 0;
    new_table->arr = malloc(sizeof(struct pair) * max_size);

    for(int i = 0; i < max_size; i++)
    {
        new_table->arr[i].key = -1;
        new_table->arr[i].val = 0;
    }
    return new_table;
}

int hash(int key, int max_size)
{
    return key % max_size;
}

void insert(hash_table table, int key)
{
    if(table == NULL)
    {
        printf("-1\n");
        return;
    }
    if(table->size == table->max_size)
    {
        printf("Table is full\n");
        return;
    }
    int index = hash(key, table->max_size);
    int i = index;

    while(table->arr[i].key != -1 && table->arr[i].key != key)
    {
        i = (i + 1) % table->size;
        if(i == index)
        {
            printf("The table is full\n");
            return;
        }
    }
    table->arr[i].key = key;
    table->arr[i].val++;
    table->size++;
}


int search(hash_table table, int key)
{
    if(table == NULL)
    {
        return -1;
    }
    int index = hash(key, table->max_size);
    int i = index;
    while(table->arr[i].key != -1)
    {
        if(table->arr[i].key == key)
        {
            return table->arr[i].val;
        }
        i = (i + 1) % table->max_size;
        if(i == index)
        {
            return 0;
        }
    }
    return 0;
}


void delete(hash_table table, int key)
{
    if(table == NULL)
    {
        printf("-1\n");
        return;
    }
    int index = hash(key, table->max_size);
    int i = index;
    while(table->arr[i].key != -1)
    {
        if(table->arr[i].key == key)
        {
            table->arr[i].key = -1;
            table->arr[i].val = 0;
            table->size--;
            return;
        }
        i = (i + 1) % table->max_size;
        if(i == index)
        {
            printf("The key is not found\n");
            return;
        }
    }
}

int max(int a, int b)
{
    if(a > b)
        return a;
    return b;
}

void unionAB(int A[], int m, int B[], int n)
{
    int max_size = 2 * max(m, n);
    int arr[1000];
    int j = 0;
    hash_table table = createHashTable(max_size);
    for(int i = 0; i < m; i++)
    {
        if (search(table, A[i]) == 0) 
        {
            arr[j++] = A[i];
            insert(table, A[i]);
        }
    }
    for(int i = 0; i < n; i++)
    {
        if(search(table, B[i]) == 0)
        {
            arr[j++] = B[i];
            insert(table, B[i]);
        }
    }
    if(j == 0)
    {
        printf("-1");
    }
    else
    {
        for(int i = 0; i < j; i++)
        {
            printf("%d ", arr[i]);
        }
    }
    printf("\n");
}

void intersectionAB(int A[], int m, int B[], int n)
{
    int max_size = 2 * max(m, n);
    int arr[1000];
    int j = 0;
    hash_table table = createHashTable(max_size);
    for(int i = 0; i < m; i++)
    {
        insert(table, A[i]);
    }
    for(int i = 0; i < n; i++)
    {
        insert(table, B[i]);
    }
    for(int i = 0; i < m; i++)
    {
        if(search(table, A[i]) > 1)
        {
            // Had to improvise in the last minute but it def has a better solution
            int flag = 0;
            for(int k = 0; k < n; k++)
            {
                if(A[i] == B[k])
                {
                    flag = 1;
                }
            }
            if(flag == 1)
            {
                arr[j++] = A[i];
                delete(table, A[i]);
            }
        }
    }
    if(j == 0)
    {
        printf("-1");
    }
    else
    {
        for(int i = 0; i < j; i++)
        {
            printf("%d ", arr[i]);
        }
    }
    printf("\n");
}

void setDifferenceAB(int A[], int m, int B[], int n)
{
    int max_size = 2 * max(m, n);
    hash_table table = createHashTable(max_size);
    int arr[1000];
    int j = 0;
    for(int i = 0; i < m; i++)
    {
        insert(table, A[i]);
    }
    for(int i = 0; i < n; i++)
    {
        insert(table, B[i]);
    }
    for(int i = 0; i < m; i++)
    {
        if(search(table, A[i]) == 1)
        {
            arr[j++] = A[i];
        }
    }
    if(j == 0)
    {
        printf("-1");
    }
    else
    {
        for(int i = 0; i < j; i++)
        {
            printf("%d ", arr[i]);
        }
    }
    printf("\n");
}

int main()
{
    int m = 5;
    int n = 6;
    scanf("%d %d", &m, &n);
    int *A = (int*)malloc(sizeof(int) * m); 
    int *B = (int*)malloc(sizeof(int) * n); 
    for (int i = 0; i < m; i++) 
    { 
        scanf("%d", &A[i]);
    }
    for (int i = 0; i < n; i++) 
    {
        scanf("%d", &B[i]);
    }
    char op, x, y;
    while (1) 
    {
        scanf("%c", &op);
        if (op == 'u') 
        {
            scanf(" %c %c", &x, &y);
            if (x == 'A' && y == 'B') 
            {
                unionAB(A, m, B, n);
            } 
            else if (x == 'B' && y == 'A') 
            {
                unionAB(B, n, A, m);
            }
        }
        else if (op == 'i') 
        {
            scanf(" %c %c", &x, &y);
            if (x == 'A' && y == 'B') 
            {
                intersectionAB(A, m, B, n);
            } 
            else if (x == 'B' && y == 'A') 
            { 
                intersectionAB(B, n, A, m); 
            }
        } 
        else if (op == 's') 
        {
            scanf(" %c %c", &x, &y);
            if (x == 'A' && y == 'B') 
            {
                setDifferenceAB(A, m, B, n);
            } 
            else if (x == 'B' && y == 'A') 
            {
                setDifferenceAB(B, n, A, m);
            }
        } 
        else if (op == 'e') 
        {
            break;
        }
    }
    return 0;
}