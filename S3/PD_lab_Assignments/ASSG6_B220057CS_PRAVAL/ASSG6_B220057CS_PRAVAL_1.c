#include<stdio.h>
#include<stdlib.h>

struct stack {
    int top;
    char arr[100000];
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

int push (struct stack *s, int n, char C)
{
    if (stack_full(s, n) == -1)
    {
        printf("%d\n", -1);
    }
    else
    {
        s->top++;
        s->arr[s->top] = C;
    }
    return 0;
}

int pop (struct stack *s)
{
    if (stack_empty(s) == -1)
    {
        printf("%d\n", -1);
    }
    else
    {
        printf("%c\n", s->arr[s->top]);
        s->top--;
    }
    return 0;
}

int peek (struct stack *s)
{
    if (stack_empty(s) == -1)
    {
        printf("%d\n", -1);
    }
    else
    {
        printf("%c\n", s->arr[s->top]);
    }
    return 0;
}


int main()
{
    int size;
    scanf("%d\n",  &size);
    char input[4], key, C;
    struct stack s;
    s.top = -1;
    while(1)
    {
        fgets(input, 4, stdin);
        sscanf(input, "%c %c", &key, &C);
        if (key == 'i')
        {
            push(&s, size, C);
            
        }
        if (key == 'd')
        {
            pop(&s);
        }
        if (key == 'p')
        {
            peek(&s);
        }
        if (key == 't')
        {
            break;
        }
    }
    return 1;
}