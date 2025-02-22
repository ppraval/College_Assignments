#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

struct node
{
    int val;
    int key;
    struct node* left;
    struct node* right;
    int height;
};

typedef struct node* node;

node createNode(int key, int val)
{  
    node new_node = (node)malloc(sizeof(struct node));
    new_node->key = key;
    new_node->val = val;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->height = 1;
    return new_node;
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
    printf("%d ", root->key);
    inorderTraversal(root->right);
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

node insertAVL(node root, int key, int val, int* count)
{
    node new_node = createNode(key, val);
    if(root == NULL)
    {
        (*count)++;
        return new_node;
    }
    if(root->key == key)
        root->val = val;
    if(root->key > key)
        root->left = insertAVL(root->left, key, val, count);
    else if(root->key < key)
        root->right = insertAVL(root->right, key, val, count);
    else
        return root;

    root->height = max(height(root->left), height(root->right)) + 1;
    int balance_factor = getBalance(root);

    if(balance_factor > 1 && key < root->left->key)
        return rightRotate(root);
    if(balance_factor < -1 && key > root->right->key)
        return leftRotate(root);
    if(balance_factor > 1 && key > root->left->key)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if(balance_factor < -1 && key < root->right->key)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

// void lowerBound(node root, int key)
// {
//      Gpt code as usual doesn't work
//     if(key == 5)
//     {
//         printf("We start here\n");
//     }
//     node traverse_node = root;
//     node lower = NULL;
//     while (traverse_node != NULL) 
//     {
//         if (key == traverse_node->key) 
//         {
//             lower = traverse_node;
//             break;
//         }
//         else if (key < traverse_node->key) 
//         {
//             lower = traverse_node;
//             traverse_node = traverse_node->left;
//         }
//         else 
//         {
//             traverse_node = traverse_node->right;
//         }
//     }
//     if (lower != NULL) 
//     {
//         printf("%d %d\n", lower->key, lower->val);
//     }
//     else 
//     {
//         printf("-1\n");
//     }
// }

void findLowerBound(node root, int key, int* flag) 
{
    if (root == NULL || *flag == 1) 
    {
        return;
    }
    if (root->key >= key) 
    {
        *flag = 1;
        printf("%d %d\n", root->key, root->val);
        return findLowerBound(root->left, key, flag);
    }
    findLowerBound(root->right, key, flag);
}

void lowerBound(node root, int key)
{
    int flag = 0;
    findLowerBound(root, key, &flag);
    if(flag == 0)
        printf("-1\n");
}

void search(node root, int key, int* flag, int* found_val, int* found_key)
{
    if(root == NULL || *flag == 1)
    {
        return;
    }
    if(root->key == key)
    {
        *flag = 1;
        *found_key = root->key;
        *found_val = root->val;
        return;
    }
    if(root->key > key)
        search(root->left, key, flag, found_val, found_key);
    if(root->key < key)
        search(root->right, key, flag, found_val, found_key);
}

void find(node root, int key)
{
    int flag = 0;
    int found_val, found_key;
    search(root, key, &flag, &found_val, &found_key);
    if(flag == 0)
    {
        printf("-1\n");
    }
    else
    {
        printf("%d %d\n", found_key, found_val);
    }
}

void empty(node root)
{
    if(root == NULL)
    {
        printf("1\n");
    }
    else
    {
        printf("0\n");
    }
}

void printElements(node root)
{
    if(root == NULL)
        printf("-1\n");
    else
        inorderTraversal(root);
    printf("\n");
}

int main()
{
    node root = NULL;
    int count = 0;
    char ch;
    int key, val;
    while(1)
    {
        scanf("%c", &ch);
        if(ch == 'i')
        {
            scanf(" %d %d", &key, &val);
            root = insertAVL(root, key, val, &count);
        }
        else if(ch == 'l')
        {
            scanf(" %d", &key);
            lowerBound(root, key);
        }
        else if(ch == 'f')
        {
            scanf(" %d", &key);
            find(root, key);
        }
        else if(ch == 's')
        {
            printf("%d\n", count);
        }
        else if(ch == 'e')
        {
            empty(root);
        }
        else if(ch == 'p')
        {
            printElements(root);
        }
        else if(ch == 't')
        {
            break;
        }
    }
    return 0;
}
