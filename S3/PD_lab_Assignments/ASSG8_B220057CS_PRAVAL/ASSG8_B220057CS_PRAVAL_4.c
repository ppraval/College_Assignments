#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct valAndIndex
{
    int val;
    int index;
};

struct node
{
    int key;
    struct node* right;
    struct node* left;
    struct node* parent;
};

typedef struct valAndIndex *vi;

typedef struct node *node;

node createNode(int key)
{
    node new_node = (node)malloc(sizeof(struct node));
    new_node->key = key;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    return new_node;
}

vi intInBrac(node root, char arr[], int i)
{
    int val = 0;
    int j;
    vi v1 = (vi)malloc(sizeof(struct valAndIndex));
    vi vtemp;
    
    for(j = i + 1; j < strlen(arr); j++)
    {
        if(arr[j] == ' ')
            continue;
        if(arr[j] == '(' || arr[j] == ')' )
            break;
    
        val = (val * 10) + (arr[j] - '0');
    }
    
    if(arr[j] == ')' || j >= strlen(arr))
    {
        v1->val = val;
        v1->index = j + 1;
        return v1;
    }
    
    printf("%d\n", val);
    root->key = val;
    root->left = createNode(0);
    vi left = intInBrac(root->left, arr, j + 1);
    printf("%d - L\n", left->val);
    root->right = createNode(left->val);
    vi right = intInBrac(root->right, arr, left->index + 1);
    printf("%d - R\n", right->val);
 
    v1->index = right->index + 2;
    v1->val = val;
    
    //if(val != 0)
    //    printf("value is %d and index is %d\n", v1->val, v1->index);
    return v1;
}

void inorderTraversal(node root)
{
    if(root == NULL)
    {
        return;
    }
    inorderTraversal(root->left);
    if(root->key != 0)
        printf("%d ", root->key);
    inorderTraversal(root->right);
}

int main()
{
    char string[] = "( 50 ( 3 ( 8 ( ) ( ) ) ( 4 ( ) ( ) ) ) ( 6 ( ) ( ) ) )";
    int i = 0;
    node root = createNode(0);
    vi v1 = intInBrac(root, string, i);
    inorderTraversal(root);
    return 1;
}