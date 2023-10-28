#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct stack 
{
    int top;
    char* arr[100];
};

typedef struct stack *stack;

stack createStack(stack s)
{
    s->top = -1;
    return s;
}

stack push(stack s, char* n)
{

    s->top++;
    s->arr[s->top] = n;
 
    return s;
}

char* pop (stack s)
{
    char* temp;
    if (s->top == -1)
    {
        printf("-1\n");
    }
    else
    {
        temp = s->arr[s->top];
        s->top--;
    }
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
            printf("%s", s->arr[i]);
        }
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

char* postfixToInfix(char postfix[], stack s)
{
    char* final;
    for(int i = 0; i < strlen(postfix); i++)
    {   
        final = (char*)malloc(sizeof(char) * 100);
        char ch = postfix[i];
        if(isOperator(ch) == 0)
        {
            char* op = (char *)malloc(sizeof(char) * 2);
            op[0] = ch;
            op[1] = '\0';
            s = push(s, op);
        }
        else
        {
            char *s1, *s2;
            s2 = pop(s);
            s1 = pop(s);
            if(i == strlen(postfix) - 1)
            {
                sprintf(final, "%s%c%s", s1, ch, s2);
            }
            else
            {
                sprintf(final, "(%s%c%s)", s1, ch, s2);
            }
            s = push(s, final);
        }
    }
    return final;
}

int main()
{
    char postfix[100];
    scanf("%s", postfix);
    stack s = (stack)malloc(sizeof(struct stack)); 
    s = createStack(s);
    char* infix = postfixToInfix(postfix, s);
    puts(infix);
    return 1;
}