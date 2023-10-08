#include<stdio.h>
#include<stdlib.h>

struct stack {
    int top;
    int arr[1000];
};

int stack_empty(struct stack *s) 
{
    if (s->top == -1) 
    {
        return -1;
    }
    else 
    {
        return 1;
    }
}

int stack_full(struct stack *s, int n) 
{
    if (s->top == n-1) 
    {
        return -1; 
    }
    else 
    {
        return 1;
    }
}

int push (struct stack *s, int n, int x)
{
    s->top++;
    s->arr[s->top] = x;
    return 0;
}

int pop (struct stack *s)
{
    int x = s->arr[s->top];
    s->top--;
    return x;
}

int peek(struct stack *s)
{
    if(stack_empty(s) == -1)
    {
        return -1;
    }
    else
    {
        return s->arr[s->top];
    }
}

int main()
{
    int size;
    scanf("%d",  &size);
    int arr[1000];
    
    struct stack *s = malloc(sizeof(struct stack));
    s->top = -1;
    int *next_greater = malloc(size);
    int counter = 0;
    
    int current, next;
    for(int i = 0; i < size; i++)
    {
        scanf("%d", &arr[i]);
    }

    for(int i = 0; i < size; i++)
    {
        while(stack_empty(s) != -1 && arr[peek(s)] < arr[i])
        {
            int x = pop(s);
            next_greater[x] = arr[i];
        }
        
        push(s, size, i);
    }
    
    while(stack_empty(s) != -1)
    {
        int x = pop(s);
        next_greater[x] = -1;
    }    

    for(int i = 0; i < size; i++)
    {
        printf("%d", next_greater[i]);
        if(i != size-1)
            printf(" ");
    } 
    return 1;
}