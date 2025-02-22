#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct stack
{
    char arr[1000];
    int top;
};

typedef struct stack* stack;

stack createStack(stack s)
{
    s->top = -1;
    return s;
}

stack push(stack s, char ch)
{
    s->top++;
    s->arr[s->top] = ch;
    return s;
}

stack pop(stack s)
{
    s->top--;
    return s;
}

int isOperator(char ch)
{
    if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^' || ch == '(' || ch == ')')
		return 1;
    else
		return 0;
}

int precedence(char ch)
{
    switch (ch)
    {
	case '+':
	case '-':
	    return 1;
	case '*':
	case '/':
	case '%':
	    return 2;
	case '^':
	    return 3;
    }
}

void printStack(stack s)
{
	for(int i = 0; i < s->top; i++)
	{	
		printf("%c", s->arr[i]);
	}
	printf("\n");
}

int main()
{
    stack s = (stack)malloc(sizeof(struct stack));
    s = createStack(s);
	char arr[10000];
	int n;
	scanf("%d\n", &n);
	fgets(arr, n + 1, stdin);	
    for(int i = 0; i < n; i++)
    {
    	if(isOperator(arr[i]))
    	{
    		if(arr[i] != ')')
    		{
	    		s = push(s, arr[i]);
	    	}
	    	else
	    	{
				if(s->arr[s->top] == '(')
				{
					printf("1\n");
					return 0;	
				}
				while(s->arr[s->top] != '(')
				{
					s = pop(s);
				}
				s = pop(s);
			}
    	}
    }
    while(s->top != -1)
    {
		s = pop(s);
    }
    printf("0\n");
    return 1;
}

