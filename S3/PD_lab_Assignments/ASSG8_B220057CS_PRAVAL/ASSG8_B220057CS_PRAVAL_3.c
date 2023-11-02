#include<stdio.h>
#include<stdlib.h>

struct node
{
    int key;
    struct node *left;
    struct node *right;
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
        root->right = insertNode(key, root->right);
        root->right->parent = root;
    }
    if(key < root->key)
    {
        root->left = insertNode(key, root->left);
        root->left->parent = root;
    }
    return root;
}

node searchNode(int key, node root)
{
    if(root == NULL || root->key == key)
    {
        return root;
    }
    if(root->key > key)
    {
        return searchNode(key, root->left);
    }
    else if (root->key < key)
    {
        return searchNode(key, root->right);
    }
}

// void parentValue(int key, node root)
// {
//     node temp = searchNode(key, root);
//     node temp_parent = temp->parent;
//     printf("%d\n", temp_parent->key);
// }

node minNode(node root)
{
    node traverse_node = root;
    while(traverse_node->left != NULL)
    {
        traverse_node = traverse_node->left;
    }
    return traverse_node;
}

node maxNode(node root)
{
    node traverse_node = root;
    while(traverse_node->right != NULL)
    {
        traverse_node = traverse_node->right;
    }
    return traverse_node;
}

void successorNode(int key, node root)
{
    if(root == NULL || searchNode(key, root) == NULL)
    {
        printf("-1\n");
        return;
    }
    node temp = searchNode(key, root);
    if(temp->right != NULL)
    {  
        node new_node = minNode(temp->right);
        printf("%d\n", new_node->key);
        return;
    }
    else
    {
        node parent = temp->parent;
        while(parent != NULL && temp == parent->right)
        {
            temp = parent;
            parent = parent->parent;
        }
        printf("%d\n", parent->key);
        return;
    }
}

node successorOfNode(int key, node root)
{
   
    node temp = searchNode(key, root);
    if(temp->right != NULL)
    {  
        node new_node = minNode(temp->right);
        return new_node;
    }
    else
    {
        node parent = temp->parent;
        while(parent != NULL && temp == parent->right)
        {
            temp = parent;
            parent = parent->parent;
        }
        return parent;
    }
}

void predecessorNode(int key, node root)
{
    if(root == NULL || searchNode(key, root) == NULL)
    {
        printf("-1\n");
        return;
    }
    node temp = searchNode(key, root);
    if(temp->left != NULL)
    {  
        node new_node = maxNode(temp->left);
        printf("%d\n", new_node->key);
        return;
    }
    else
    {
        node parent = temp->parent;
        while(parent != NULL && temp == parent->left)
        {
            temp = parent;
            parent = parent->parent;
        }
        printf("%d\n", parent->key);
        return;
    }
}

node deleteNode(int key, node root)
{
    if(root == NULL || searchNode(key, root) == NULL)
    {
        printf("-1\n");
        return root;
    }
    node temp = searchNode(key, root);
    if(temp->right == NULL && temp->left == NULL)
    {
        if(temp == temp->parent->right)
        {
            temp->parent->right = NULL;
            return root;
        }
        else if(temp == temp->parent->left)
        {
            temp->parent->left = NULL;
            return root;
        }
    }  
    if(temp->right == NULL)
    {
        temp->key = temp->left->key;
        temp->left = NULL;
        return root;
    }
    if(temp->left == NULL)
    {
        temp->key = temp->right->key;
        temp->right = NULL;
        return root;
    }
    if(temp->right != NULL && temp->left != NULL)
    {
        node new_temp = successorOfNode(key, root);
        int temp_key = new_temp->key;
        root = deleteNode(new_temp->key, root);
        temp->key = temp_key;
        return root;
    }
}

void preorderTraversal(node root)
{
    if(root == NULL)
    {
        printf("( ) ");
        return;
    }
    printf("( ");
    printf("%d ", root->key);
    preorderTraversal(root->left);
    preorderTraversal(root->right);
    printf(") ");
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
        if(cmd == 'i')
        {
            root = insertNode(key, root);
        }
        // if(cmd == 'd')
        // {
        //     root = deleteNode(key, root);
        // }
        if(cmd == 'p')
        {
            if(root == NULL)
            {
                printf("( )\n");
            }
            else 
            {
                preorderTraversal(root);
                if(root != NULL)
                {
                    printf("\n");
                }
            }
        }
        if(cmd == 'e')
        {
            break;
        }
    }
    return 1;
}