#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct stack 
{
    int size;
    int top;
    char arr[100];
};

typedef struct stack *stack;

stack createStack(stack s, int size)
{
    s->size = size;
    s->top = -1;
    return s;
}

stack push(stack s, char n)
{
    if(s->top >= s->size - 1)
    {
        printf("-1\n");
    }
    else
    {
        s->top++;
        s->arr[s->top] = n;
    }
    return s;
}

char pop (stack s)
{
    char temp;
    if (s->top == -1)
    {
        printf("-1\n");
    }
    else
    {
        temp =  s->arr[s->top];
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
            printf("%c ", s->arr[i]);
        }
    }
}

int precedence(char op)
{
    switch (op) 
    {
        case '^':
            return 6;
        case '+':
        case '-':
            return 9;
        case '*':
        case '/':
            return 7;
        case '%':
        case '#':
            return 10;
        default:
            return -1;
    }
}

int isOperator(char ch)
{
    if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^' || ch == '#')
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}

void infixToPostfix(int n, char infix[], char postfix[], stack s)
{
    int j = 0;
    for(int i = 0; i < n; i++)
    {
        if(infix[i] != ' ')
        {
            if(infix[i] == '(')
            {
                s = push(s, '(');
            }
            else if(infix[i] == ')')
            {
                while(1)
                {
                    char ch = pop(s);
                    if(ch == '(')
                    {
                        break;
                    }
                    else
                    {

                        postfix[j++] = ch;
                    }
                }
            }
            else
            {
                if(isOperator(infix[i]) == 0)
                {
                    postfix[j++] = infix[i];
                }
                else
                {   
                    if (s->top == -1)
                    {
                        s = push(s, infix[i]);
                    }
                    else
                    {
                        while(s->top > -1)
                        {
                            
                            if((precedence(s->arr[s->top]) >= precedence(infix[i])))
                            {
                                postfix[j++] = pop(s);
                                s = push(s, infix[i]);
                                break;
                            }
                            else
                            {
                                s = push(s, infix[i]);
                                break;
                            }
                        }
                    }    
                }
            }
        }
    }
    while(s->top > -1)
    {
        postfix[j++] = pop(s);
    }
    postfix[j] = '\0';
}

int main()
{
    char infix[100], postfix[100];
    fgets(infix, 100, stdin);
    stack s = (stack)malloc(sizeof(struct stack)); 
    s = createStack(s, strlen(infix));
    
    infixToPostfix(strlen(infix), infix, postfix, s);
    for(int i = 0; i < strlen(postfix); i++)
    {
        if(postfix[i] != '\n')
        {
            if(i == 0)
            printf("%c", postfix[i]);
            else
            printf(" %c", postfix[i]);
        }
    }
    return 1;
}
