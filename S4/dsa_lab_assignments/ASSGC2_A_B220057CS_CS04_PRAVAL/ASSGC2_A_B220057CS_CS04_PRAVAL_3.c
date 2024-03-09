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

node createNode(int val)
{
    node new_node = (node)malloc(sizeof(struct node));
    new_node->val = val;
    new_node->parent = NULL;
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->colour = 'R';
    return new_node;
}

node leftRotate(node x)
{
    node y = x->right;
    node T2 = y->left;

    y->left = x;
    x->right = T2;
    y->parent = x->parent;
    x->parent = y;
    if(T2 != NULL)
        T2->parent = x;
    return y;
}

node rightRotate(node x)
{
    node y = x->left;
    node T2 = y->right;

    y->right = x;
    x->left = T2;
    y->parent = x->parent;
    x->parent = y;
    if(T2 != NULL)
        T2->parent = x;
    return y;
}

node insert(node root, int val, int* ll, int* rr, int* rl, int* lr)
{
    int flag = 0;

    node new_node = createNode(val);
    if(root == NULL)
        return new_node;
    else if (root->val > val)
    {
        root->left = insert(root->left, val, ll, rr, rl, lr);
        root->left->parent = root;
        if(root->parent != NULL)
        {
            if(root->colour == 'R' && root->left->colour == 'R')
            {
                flag = 1;
            }   
        }
    }
    else
    {
        root->right = insert(root->right, val, ll, rr, rl, lr);
        root->right->parent = root;
        if(root->parent != NULL)
        {
            if(root->colour == 'R' && root->right->colour == 'R')
            {
                flag = 1;
            }   
        }
    }

    if (*ll == 1) 
    {
        root = leftRotate(root);
        root->colour = 'B';
        root->left->colour = 'R';
        *ll = 0;
    } 
    else if (*rr == 1) 
    {
        root = rightRotate(root);
        root->colour = 'B';
        root->right->colour = 'R';
        *rr = 0;
    } 
    else if (*rl == 1) 
    {
        root->right = rightRotate(root->right);
        root->right->parent = root;
        root = leftRotate(root);
        root->colour = 'B';
        root->left->colour = 'R';
        *rl = 0;
    } 
    else if (*lr == 1) 
    {
        root->left = leftRotate(root->left);
        root->left->parent = root;
        root = rightRotate(root);
        root->colour = 'B';
        root->right->colour = 'R';
        *lr = 0;
    }


    if(flag == 1)
    {
        if(root->parent->right == root)
        {
            if(root->parent->left == NULL || root->parent->left->colour == 'B')
            {
                if(root->left != NULL && root->left->colour == 'R')
                {
                    *rl = 1;
                }
                else if(root->right != NULL && root->right->colour == 'R')
                {
                    *ll = 1;
                }
            }
            else
            {
                root->parent->left->colour = 'B';
                root->colour = 'B';
                if(root->parent->parent != NULL)
                {
                    root->parent->colour = 'R';
                }
            }
        }
        else
        {
            if(root->parent->right == NULL || root->parent->right->colour == 'B')
            {
                if(root->left != NULL && root->left->colour == 'R')
                {
                    *rr = 1;
                }
                else if(root->right != NULL && root->right->colour == 'R')
                {
                    *lr = 1;
                }
            }
            else
            {
                root->parent->right->colour = 'B';
                root->colour = 'B';
                if(root->parent->parent != NULL)
                {
                    root->parent->colour = 'R';
                }
            }
        }
        flag = 0;
    }
    return root;
}

node insertRedBlack(node root, int val)
{
    if(root == NULL)
    {
        root = createNode(val);
        root->colour = 'B';
    }
    else
    {
        int ll = 0, rr = 0, rl = 0, lr = 0;
        root = insert(root, val, &ll, &rr, &rl, &lr);
    }
    return root;
}

void inorder(node root)
{
    if(root == NULL)
    {
        return;
    }
    inorder(root->left);
    printf("%d - %c ", root->val, root->colour);
    if(root->parent != NULL)
        printf("- (P) ");
    inorder(root->right);
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

int main()
{
    node root = NULL;
    char arr[10];
    while(1)
    {
        int i, val = 0;
        for(i = 0; i < 10; i++)
        {
            scanf("%c", &arr[i]);
            if(arr[i] == '\n')
            {
                break;
            }
        }
        arr[i] = '\0';
        if(arr[0] == 'e')
        {
            break;
        }
        i = 0;
        while(i < strlen(arr))
        {
            val = val * 10 + arr[i] - '0';
            i++;
        }
        root = insertRedBlack(root, val);
        preorder(root);
        printf("\n");
    }
    return 0;
}