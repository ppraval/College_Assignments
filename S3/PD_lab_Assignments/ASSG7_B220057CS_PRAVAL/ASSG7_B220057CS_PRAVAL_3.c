#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct stack 
{
    int top;
    int arr[100];
};

typedef struct stack *stack;

stack createStack(stack s)
{
    s->top = -1;
    return s;
}

stack push(stack s, int n)
{
    s->top++;
    s->arr[s->top] = n;
    return s;
}

int pop (stack s)
{
    int temp;
        temp =  s->arr[s->top];
        s->top--;
    return temp;
}

void printStack(stack s)
{
    if(s->top == -1)
    {
        printf("-1\n");
    }
    else
    {
        for(int i = 0; i <= s->top; i++)
        {
            printf("%d ", s->arr[i]);
        }
    }
}

int calculation(char op, int a, int b)
{
    switch (op) 
    {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default:
            return -1;
    }
}

int isOperator(char ch)
{
    if(ch == '+' || ch == '-' || ch == '*' || ch == '/')
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}

void evaluatePostfix(char expression[], stack s)
{
    int initial = 0;
    for(int i = 0; i < strlen(expression); i++)
    {
        char *temp = (char*)malloc(sizeof(char) * 100);
        if(expression[i] == ' ' || expression[i] == '\n')
        {
            int k = 0;
            if(isOperator(expression[i - 1]) == 1)
            {
                initial = i + 1;
                continue;
            }
            else
            {
                for(int j = initial; j < i; j++)
                {
                    temp[k++] = expression[j];
                }
            }
            temp[k++] = '\0';
            s = push(s, atoi(temp));
            initial = i + 1;
            continue;
        }
        else if (isOperator(expression[i]))
        {
            if(s->top < 1)
            {
                pop(s);
                break;
            }
            else
            {
                int b = pop(s);
                int a = pop(s);
                s = push(s, calculation(expression[i], a, b));
            }
        }
    }
    if(s->top == 0)
    {
        printf("%d", pop(s));
    }
    else
    {
        printf("-1");
    }
}

int main()
{
    int n, j = 0;
    char postfix[100];
    fgets(postfix, 100, stdin);
    n = strlen(postfix);
    stack s = (stack)malloc(sizeof(struct stack));
    s = createStack(s);
    evaluatePostfix(postfix, s);
    return 1;
}