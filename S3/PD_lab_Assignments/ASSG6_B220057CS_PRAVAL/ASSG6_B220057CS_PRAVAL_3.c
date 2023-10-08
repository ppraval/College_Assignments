#include<stdio.h>
#include<stdlib.h>

struct queue {
    int head;
    int tail;
    char *arr[100000];
};

int queue_full(struct queue *q, int n)
{
    if ((q->tail + 1) % n == q->head)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

int queue_empty(struct queue *q)
{
    if (q->head == -1)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

int enqueue(struct queue *q, char *s, int n)
{
    if (queue_full(q, n) == -1)
    {
        printf("%d\n", -1);
    }
    else
    {
        if(q->head == -1)
            q->head = 0;
        q->tail = (q->tail + 1) % n;
        q->arr[q->tail] = s;
    }
    return 0;
}

int dequeue(struct queue *q, int n)
{
    if (queue_empty(q) == -1)
    {
        printf("%d\n", -1);
    }
    else
    {
        printf("%s\n", q->arr[q->head]);
        if(q->head == q->tail)
        {
            q->head = -1;
            q->tail = -1;
        }
        else
        {
            q->head = (q->head + 1) % n;
        }
    }
    return 0;
}

int main()
{
    int size;
    scanf("%d\n", &size);
    char input[23], key;
    struct queue *q = malloc(sizeof(struct queue));
    q->head = -1;
    q->tail = -1;
    while(1)
    {
        char *str = malloc(21);
        fgets(input, 23, stdin);
        sscanf(input, "%c %s", &key, str);
        if (key == 'i')
        {
            enqueue(q, str, size);
        }
        if (key == 'd')
        {
            dequeue(q, size);
        }
        if (key == 'f')
        {
            printf("%d\n", queue_full(q, size));
        }
        if (key == 'e')
        {
            printf("%d\n", queue_empty(q));
        }
        if (key == 't')
        {
            break;
        }
    }
    return 1;
}