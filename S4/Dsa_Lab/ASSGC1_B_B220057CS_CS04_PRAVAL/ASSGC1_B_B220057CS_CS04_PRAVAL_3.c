#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

struct node
{
    int val;
    struct node* right;
    struct node* left;
};

typedef struct node *node;

struct queue
{
    node arr[1000];
    int front;
    int last;
};

typedef struct queue *queue;

queue createQueue()
{
    queue q = (queue)malloc(sizeof(struct queue));
    q->front = 0;
    q->last = 0;
    return q;
}

queue push(queue q, node tree_node)
{
    q->arr[q->last++] = tree_node;
    return q;
}

queue pop(queue q)
{
    q->front++;
    return q;
}

node queueFront(queue q)
{
    node top_node = q->arr[q->front];
    return top_node;
}

int queueEmpty(queue q)
{
    if(q->front == q->last)
        return 1;
    return 0;
}

node createNode(int val)
{
    node new_node = (node)malloc(sizeof(struct node));
    new_node->val = val;
    new_node->right = NULL;
    new_node->left = NULL;
    return new_node;
}

node createTreeFromBrackets(char s[], int* i)
{
    int n = strlen(s);
    if(*i >= n || n == 0)
    {
        return NULL;
    }
    int val = 0;
    while(s[*i] != '(' && s[*i] != ')' && *i < n)
    {
        int num = s[*i] - '0';
        val = val * 10 + num;
        (*i)++;
    }
    node root = NULL;
    if(val > 0)
    {
        root = createNode(val);
    }

    if(*i < n && s[*i] == '(')
    {
        (*i)++;
        root->left = createTreeFromBrackets(s, i);
    }
    if(*i < n && s[*i] == ')')
    {
        (*i)++;
        return root;
    }

    if(*i < n && s[*i] == '(')
    {
        (*i)++;
        root->right = createTreeFromBrackets(s, i);
    }
    if(*i < n && s[*i] == ')')
    {
        (*i)++;
        return root;
    }
    return root;
}

void printRangeValues(node root, int k1, int k2)
{
    if(root == NULL)
    {
        return;
    }
    printRangeValues(root->left, k1, k2);
    if(root->val >= k1 && root->val <= k2)
        printf("%d ", root->val);
    printRangeValues(root->right, k1, k2);
}

void ancestorsOfNode(node root, int val, int* flag)
{
    if(root == NULL || *flag == 1)
    {
        return;
    }
    ancestorsOfNode(root->left, val, flag);
    ancestorsOfNode(root->right, val, flag);
    if(val == root->val || *flag == 1)
    {
        printf("%d ", root->val);
        *flag = 1;
        return;
    }
}

int main()
{
    char* arr = (char*)malloc(1000000 * sizeof(char));
    node root = NULL;
    int i = 0, flag = 0;
    scanf("%[^\n]s", arr);
    root = createTreeFromBrackets(arr, &i);
    while (1)
    {   
        char ch;
        scanf("%c", &ch);
        if(ch == 'p')
        {
            int k1, k2;
            scanf("%d%d", &k1, &k2);
            printRangeValues(root, k1, k2);
            printf("\n");
        }
        else if(ch == 'a')
        {
            int val;
            scanf("%d", &val);
            ancestorsOfNode(root, val, &flag);
            printf("\n");
        }
        else if(ch == 'e')
        {
            break;
        }
    }
    return 1;
}