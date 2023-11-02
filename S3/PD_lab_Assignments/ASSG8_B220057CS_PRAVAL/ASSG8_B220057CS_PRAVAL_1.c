#include<stdio.h>
#include<stdlib.h>

struct node
{
    int key;
    struct node *right;
    struct node *left;
    struct node *parent;
};

typedef struct node *node;

node createNode(int key)
{
    node new_node = (node)malloc(sizeof(struct node));
    new_node->key = key;
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->parent = NULL;
    return new_node;
}

node insertNode(int key, node root)
{   
    node new_node = createNode(key);
    if(root == NULL)
    {
        return new_node;
    }
    if(key > root->key)
    {
        root->left = insertNode(key, root->left);
        root->left->parent = root;
    }
    if(key < root->key)
    {
        root->right = insertNode(key, root->right);
        root->right->parent = root;
    }
    return root;
}

void inorderTraversal(node root)
{
    if(root == NULL)
    {
        return;
    }
    inorderTraversal(root->left);
    // if(root->right != NULL || root->left != NULL)
    // {
    //     printf("%d", root->key);
    // }
    // else
    // {
    //     printf(" %d ", root->key);
    // }
    printf("%d ", root->key);
    inorderTraversal(root->right);
}

void postorderTraversal(node root)
{
    if (root == NULL)
    {
        return;
    }
    postorderTraversal(root->left);
    postorderTraversal(root->right);
    if(root->parent == NULL)
    {
        printf("%d", root->key);
    }
    else 
    {
        printf("%d ", root->key);
    }
}

void preorderTraversal(node root)
{
    if(root == NULL)
    {
        return;
    }
    else if(root->parent == NULL)
    {
        printf("%d", root->key);
    }
    else
    {
        printf(" %d", root->key);
    }
    preorderTraversal(root->left);
    preorderTraversal(root->right);
}

void searchNode(int key, node root)
{
    if(root == NULL)
    {
        return;
    }
    if (root->key != key)
    {
        printf("N\n");
        return;
    }
    if(root->key == key)
    {
        printf("F\n");
        return;
    }
    if(root->key < key)
    {
        return searchNode(key, root->left);
    }
    else if (root->key > key)
    {
        return searchNode(key, root->right);
    }

}

void minNode(node root)
{
    if(root == NULL)
    {
        return;
    }
    node traverse_node = root;
    while(traverse_node->right != NULL)
    {
        traverse_node = traverse_node->right;
    }
    printf("%d", traverse_node->key);
}

void maxNode(node root)
{
    if(root == NULL)
    {
        return;
    }
    node traverse_node = root;
    while(traverse_node->left != NULL)
    {
        traverse_node = traverse_node->left;
    }
    printf("%d", traverse_node->key);
}

int main()
{
    node root = NULL;
    int key;
    char cmd, input[1000];
    while(1)
    {
        fgets(input, 1000, stdin);
        sscanf(input, "%c %d", &cmd, &key);
        if(cmd == 'a')
        {
            root = insertNode(key, root);
        }
        if(cmd == 's')
        {
            searchNode(key, root);
        }
        if(cmd == 'x')
        {
            maxNode(root);
            if(root != NULL)
            {
                printf("\n");
            }
        }
        if(cmd == 'n')
        {
            minNode(root);
            if(root != NULL)
            {
                printf("\n");
            }
        }
        if(cmd == 'i')
        {
            inorderTraversal(root);
            if(root != NULL)
            {
                printf("\n");
            }
        }
        if(cmd == 'p')
        {
            preorderTraversal(root);
            if(root != NULL)
            {
                printf("\n");
            }
        }
        if(cmd == 't')
        {
            postorderTraversal(root);
            if(root != NULL)
            {
                printf("\n");
            }
        }
        if(cmd == 'e')
        {
            break;
        }
    }
    return 1;
}