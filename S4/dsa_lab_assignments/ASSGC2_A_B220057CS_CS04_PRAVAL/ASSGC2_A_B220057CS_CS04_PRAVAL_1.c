#include<stdio.h>
#include<stdlib.h>

struct node
{
    int val;
    struct node* left;
    struct node* right;
    int height;
};

typedef struct node* node;

node createNode(int val)
{  
    node new_node = (node)malloc(sizeof(struct node));
    new_node->val = val;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->height = 1;
    return new_node;
}

node insertBST(node root, int val)
{
    node new_node = createNode(val);
    if(root == NULL)
    {
        return new_node;
    }
    if(root->val > val)
        root->left = insertBST(root->left, val);
    if(root->val < val)
        root->right = insertBST(root->right, val);
    return root;
}

int max(int a, int b)
{
    if(a > b)
        return a;
    return b;
}

int height(node tree_node)
{
    if(tree_node == NULL)
        return 0;
    return tree_node->height;
}

void inorderTraversal(node root)
{
    if(root == NULL)
        return;
    inorderTraversal(root->left);
    printf("%d ", root->val);
    inorderTraversal(root->right);
}

void preorderTraversal(node root)
{
    if(root == NULL)
        return;
    printf("%d ", root->val);
    preorderTraversal(root->left);
    preorderTraversal(root->right);
}


node leftRotate(node x)
{
    node y = x->right;
    node T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

node rightRotate(node y)
{
    node x = y->left;
    node T2 = x->right;

    x->right = y;
    y->left = T2;
    
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

int getBalance(node tree_node)
{
    if(tree_node == NULL)
        return 0;
    return height(tree_node->left) - height(tree_node->right);
}

node insertAVL(node root, int val, int* right_count, int* left_count)
{
    node new_node = createNode(val);
    if(root == NULL)
    {
        return new_node;
    }
    if(root->val > val)
        root->left = insertAVL(root->left, val, right_count, left_count);
    else if(root->val < val)
        root->right = insertAVL(root->right, val, right_count, left_count);
    else
        return root;

    root->height = max(height(root->left), height(root->right)) + 1;
    int balance_factor = getBalance(root);

    if(balance_factor > 1 && val < root->left->val)
    {
        (*right_count)++;
        return rightRotate(root);
    }
    if(balance_factor < -1 && val > root->right->val)
    {
        (*left_count)++;
        return leftRotate(root);
    }
    if(balance_factor > 1 && val > root->left->val)
    {
        root->left = leftRotate(root->left);
        (*right_count)++;
        (*left_count)++;
        return rightRotate(root);
    }
    if(balance_factor < -1 && val < root->right->val)
    {
        root->right = rightRotate(root->right);
        (*right_count)++;
        (*left_count)++;
        return leftRotate(root);
    }
    return root;
}

int search(node root, int val)
{
    if(root == NULL)
        return 0;
    if(root->val == val)
        return 1;
    if(root->val > val)
        search(root->left, val);
    if(root->val < val)
        search(root->right, val);
}

void printPath(node root, int val)
{
    if(root == NULL)
    {
        printf("-1\n");
        return;
    }
    if(val == root->val)
    {
        printf("%d ", root->val);
    }
    if(val > root->val)
    {
        printf("%d ", root->val);
        printPath(root->right, val);
    }
    if(val < root->val)
    {
        printf("%d ", root->val);
        printPath(root->left, val);
    }
}

void searchAVL(node root, int val)
{
    if(search(root, val))
    {
        printPath(root, val);
    }
    else
    {
        printf("-1");
    }
    printf("\n");
}

node makeASampleTree()
{
    node root = NULL;
    root = insertBST(root, 5);
    root = insertBST(root, 2);
    root = insertBST(root, 3);
    root = insertBST(root, 1);
    root = insertBST(root, 4);
    root = insertBST(root, 7);
    root = insertBST(root, 6);
    root = insertBST(root, 9);
    return root;
}

int main()
{
    node root = NULL;
    int right_count = 0, left_count = 0;
    char ch;
    int val;
    while(1)
    {
        scanf("%c", &ch);
        if(ch == 'i')
        {
            scanf(" %d", &val);
            root = insertAVL(root, val, &right_count, &left_count);
        }
        else if(ch == 'x')
        {
            scanf(" %d", &val);
            searchAVL(root, val);
        }
        else if(ch == 'p')
        {
            preorderTraversal(root);
            printf("\n");
        }
        else if(ch == 's')
        {
            printf("%d %d\n", left_count, right_count);
        }
        else if(ch == 'e')
        {
            break;
        }
    }
    return 0;
}
