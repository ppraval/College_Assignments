#include<stdio.h>
#include<stdlib.h>

struct queue {
    int head;
    int tail;
    int arr[100000];
};

int size(struct queue *q)
{
    return q->head - q->tail;
}

int front(struct queue *q)
{
    return q->arr[q->head];
}

int queue_full(struct queue *q)
{
    if ((q->tail + 1) == q->head)
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
    if (q->head == q->tail)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

int enqueue(struct queue *q, int n)
{
    if (queue_full(q) == -1)
    {
        printf("%d\n", -1);
    }
    else
    {
        q->arr[q->tail] = n;
        q->tail = (q->tail + 1);
    }
    return 0;
}

int dequeue(struct queue *q)
{
    if (queue_empty(q) == -1)
    {
        printf("%d\n", -1);
    }
    else
    {
       q->head = (q->head + 1);
    }
    return 0;
}

void reverse_first_k(struct queue *q, int k)
{
    if (k == 0)
    {
        return;
    }

    int first = front(q);
    dequeue(q);
    reverse_first_k(q, k - 1);
    enqueue(q, first);
}

int add_everything_back(struct queue *q, int n, int k)
{
    reverse_first_k(q, k);
    
    int diff = n - k;
    while (diff > 0)
    {
        int temp = front(q);
        dequeue(q);
        enqueue(q, temp);
        diff--;
    }
    return 0;
}

int main()
{
    int n, k;
    scanf("%d %d", &n, &k);

    struct queue *q = malloc(sizeof(struct queue));
    q->head = 0;
    q->tail = 0;
    
    int arr[n];
    for(int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }
    for(int i = 0; i < n; i++)
    {
        enqueue(q, arr[i]);
    }

    add_everything_back(q, n, k);

    for(int i = q->head; i < q->tail; i++)
    {
        printf("%d", q->arr[i]);
        if(i != q->tail-1)
            printf(" ");
    }

    return 1;
}