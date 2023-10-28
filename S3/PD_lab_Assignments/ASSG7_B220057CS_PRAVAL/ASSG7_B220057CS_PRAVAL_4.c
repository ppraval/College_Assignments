#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node
{
    char data;
    struct node *left;
    struct node *right;
};

typedef struct node *node;

struct stack 
{
    int top;
    float arr[1000];
};

struct char_stack
{
    int top;
    char arr[1000];
};

struct node_stack 
{
    int top;
    node arr[1000];
};

typedef struct node_stack *node_stack;

typedef struct char_stack *char_stack;

typedef struct stack *stack;

stack createStack(stack s)
{
    s->top = -1;
    return s;
}

stack push(stack s, float n)
{
    s->top++;
    s->arr[s->top] = n;
    return s;
}

float pop (stack s)
{
    float temp;
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
            printf("%f ", s->arr[i]);
        }
    }
}

char_stack ccreateStack(char_stack s)
{
    s->top = -1;
    return s;
}

char_stack cpush(char_stack s, char n)
{
    s->top++;
    s->arr[s->top] = n;
    return s;
}

char cpop (char_stack s)
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

void cprintStack(char_stack s)
{
    if(s->top == -1)
    {
        printf("-1\n");
    }
    else
    {
        for(int i = 0; i <= s->top; i++)
        {
            printf("%c_", s->arr[i]);
        }
    }
}

node createNode(char data, node left, node right) 
{
    node temp;
    temp = (node)malloc(sizeof(struct node));
    temp->data = data;
    temp->left = left;
    temp->right = right;
    return temp;
}

node_stack ncreateStack(node_stack s)
{
    s->top = -1;
    return s;
}

node_stack npush(node_stack s, node n)
{
    s->top++;
    s->arr[s->top] = n;
    return s;
}

node npop(node_stack s)
{
    node temp;
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

int precedence(char op)
{
    switch (op) 
    {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
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

float calculation(char op, float a, float b)
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

void evaluatePostfix(char expression[], stack s)
{
    int initial = 0, div_zero = 0;
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
                float b = pop(s);
                float a = pop(s);
            
                if(expression[i] == '/' && b == 0)
                {
                    printf("N\n");
                    div_zero = 1;
                    break;
                }
                else
                {
                    s = push(s, calculation(expression[i], a, b));
                }
            }
        }
    }
    if(s->top == 0 && div_zero == 0)
    {
        printf("%.2f\n", pop(s));
    }
}

void infixToPostfix(int n, char* infix[], char postfix[], char_stack s)
{
    int j = 0;
    for(int i = 0; i < n; i++)
    {
        if(infix[i][0] == '(')
        {
            s = cpush(s, '(');
        }
        else if(infix[i][0] == ')')
        {
            while(1)
            {
                char ch = cpop(s);
                if(ch == '(')
                {
                    break;
                }
                else
                {
                    postfix[j++] = ch;
                    postfix[j++] = ' ';
                }
            }
        }
        else
        {
            if(isOperator(infix[i][0]) == 0)
            {
                for(int k = 0; infix[i][k] != '\0'; k++)
                {
                    postfix[j++] = infix[i][k];
                }
                postfix[j++] = ' ';
            }
            else
            {   
                if (s->top == -1)
                {
                    s = cpush(s, infix[i][0]);
                }
                else
                {
                    while(s->top > -1)
                    {
                        
                        if((precedence(s->arr[s->top]) >= precedence(infix[i][0])))
                        {
                            postfix[j++] = cpop(s);
                            postfix[j++] = ' ';
                            s = cpush(s, infix[i][0]);
                            break;
                        }
                        else
                        {
                            s = cpush(s, infix[i][0]);
                            break;
                        }
                    }
                }    
            }
        }
    }
    while(s->top > -1)
    {
        postfix[j++] = cpop(s);
    }
}

node postfixToExpTree(node_stack s, char postfix[])
{
    for(int i = 0; i < strlen(postfix); i++) 
    {
        if(postfix[i] == ' ')
        {
            continue;
        }
        if(isOperator(postfix[i]) == 0)
        {
            char data[10];
            int k = 0;
            while(postfix[i] != ' ')
            {
                data[k++] = postfix[i++]; 
            }
            data[k] = '\0';
            node temp_node = createNode(postfix[i], NULL, NULL);
            s = npush(s, temp_node);

        }
        else
        {
            node left = npop(s);
            node right = npop(s);
            node new_temp = createNode(postfix[i], left, right);
            s = npush(s, new_temp);
        }
    }
    node final_tree = npop(s);
    return final_tree;
}

int heightExpTree(node root)
{
    if(root == NULL)
    {
        return 0;
    }
    else
    {
        int left_height = heightExpTree(root->left);
        int right_height = heightExpTree(root->right);
        if(left_height >= right_height)
        {
            return left_height + 1;
        }
        else
        {
            return right_height + 1;
        }
    }
}
int minDistanceExpTree(node root)
{
    if(root == NULL)
    {
        return 0;
    }
    else
    {
        int left_height = heightExpTree(root->left);
        int right_height = heightExpTree(root->right);
        if(left_height <= right_height)
        {
            return left_height + 1;
        }
        else
        {
            return right_height + 1;
        }
    }
}

int main()
{
    char input[1000], postfix[1000] = {'\0'}, expression[1000];
    char *infixexp[1000];
    
    stack s = (stack)malloc(sizeof(struct stack)); 
    s = createStack(s);
    char_stack cs = (char_stack)malloc(sizeof(struct char_stack));
    cs = ccreateStack(cs);
    node_stack ns = (node_stack)malloc(sizeof(struct node_stack));
    ns = ncreateStack(ns);
    fgets(input, 1000, stdin);
    int j = 0;
    for(int i = 0; input[i] != '\n' && input[i] != '\0'; i++)
    {
        if(input[i] == ' ' || input[i] == '\n')
        {
            continue;
        }
        char data[10];
        int k = 0;
        while (input[i] != ' ' && input[i] != '\n' && input[i] != '\0')
        {
            data[k++] = input[i++];
        }
        data[k] = '\0';
        infixexp[j] = (char*)malloc(10);
        strcpy(infixexp[j++], data);
    }
    infixToPostfix(j, infixexp, postfix, cs);
    printf("%s\n", postfix);
    evaluatePostfix(postfix, s);
    node root = postfixToExpTree(ns, postfix);
    printf("%d\n", heightExpTree(root) - 1);
    printf("%d\n", minDistanceExpTree(root) - 1);
    return 1;
}