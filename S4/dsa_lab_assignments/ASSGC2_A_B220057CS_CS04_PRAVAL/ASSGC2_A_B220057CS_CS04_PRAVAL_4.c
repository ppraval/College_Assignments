#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node
{
    int val;
    char colour;
    struct node* parent;
    struct node* left;
    struct node* right;
};

typedef struct node* node;

node createNode(int val, char colour)
{
    node new_node = (node)malloc(sizeof(struct node));
    new_node->val = val;
    new_node->parent = NULL;
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->colour = colour;
    return new_node;
}

int max(int a, int b)
{
    if(a >= b)
        return a;
    return b;
}

void inorder(node root, int arr[], int* i)
{
    if(root == NULL)
        return;
    inorder(root->left, arr, i);
    arr[(*i)++] = root->val;
    inorder(root->right, arr, i);
}

node createTreeFromBrackets(char s[], int* i)
{
    int n = strlen(s);
    if(*i > n || n == 0)
    {
        return NULL;
    }
    if(s[*i] == ' ')
        (*i)++;
    int val = 0;
    char colour;
    while(s[*i] != '(' && s[*i] != ')'  && s[*i] != ' ' && *i < n)
    {
        int num = s[*i] - '0';
        val = val * 10 + num;
        (*i)++;
    }
    if(s[*i] == ' ')
        (*i)++;
    if(s[*i] == 'R')
    {
        colour = 'R';
        (*i)++;
    }
    if(s[*i] == 'B')
    {
        colour = 'B';
        (*i)++;
    }
    if(s[*i] == ' ')
        (*i)++;
    node root = NULL;
    if(val > 0)
    {
        root = createNode(val, colour);
    }
    if(*i < n && s[*i] == '(')
    {
        (*i)++;
        root->left = createTreeFromBrackets(s, i);
    }
    if(s[*i] == ' ')
        (*i)++;
    if(*i < n && s[*i] == ')')
    {
        (*i)++;
        return root;
    }
    if(s[*i] == ' ')
        (*i)++;
    if(*i < n && s[*i] == '(')
    {
        (*i)++;
        root->right = createTreeFromBrackets(s, i);
    }
    if(s[*i] == ' ')
        (*i)++;
    if(*i < n && s[*i] == ')')
    {
        (*i)++;
        return root;    
    }
    return root;
}

void preorder(node root)
{
    if(root == NULL)
    {
        printf("( ) ");
        return;
    }
    printf("( ");
    printf("%d %c ", root->val, root->colour);
    preorder(root->left);
    preorder(root->right);
    printf(") ");
}

void postorderTraversal(node root, int* flag)
{
    if(root == NULL || *flag == 1)
    {
        return;
    }
    postorderTraversal(root->left, flag);
    postorderTraversal(root->right, flag);
    if(root->colour == 'R')
    {
        if(root->left != NULL)
        {
            if(root->left->colour == 'R')
            {
                *flag = 1;
                return;
            }
        }
        if(root->right != NULL)
        {
            if(root->right->colour == 'R')
            {
                *flag = 1;
                return;
            }
        }
    }
}

int blackHeight(node root, int* flag)
{
    if(root == NULL)
        return 0;
    int left = blackHeight(root->left, flag);
    int right = blackHeight(root->right, flag);
    if(left != right)
    {
        *flag = 1;
    }
    if(root->colour == 'B')
        return max(left, right) + 1;
    else
        return 0;
}

void isBST(node root, int* flag)
{
    int i = 0;
    int arr[100];
    inorder(root, arr, &i);
    for(int j = 0; j + 1 < i; j++)
    {
        if(!(arr[j] <= arr[j + 1]))
        {
            *flag = 1;
            break;
        }
    }
}

int main()
{
    int i = 1;
    int flag = 0;
    char* s = (char*)malloc(sizeof(char) * 1000);
    scanf("%[^\n]s", s);
    node root = NULL;
    root = createTreeFromBrackets(s, &i);
    if(root == NULL)
    {
        printf("1");
        return 0;
    }
    else if(root->colour == 'R')
    {
        printf("0");
        return 0;
    }
    isBST(root, &flag);
    if(flag == 1)
    {
        printf("0");
        return 0;
    }
    int bh = blackHeight(root, &flag);
    if(flag == 1)
    {
        printf("0");
        return 0;
    }
    postorderTraversal(root, &flag);
    if(flag == 1)
    {
        printf("0");
        return 0;
    }
    else
    {
        printf("1");
        return 0;
    }
    return 0;
}