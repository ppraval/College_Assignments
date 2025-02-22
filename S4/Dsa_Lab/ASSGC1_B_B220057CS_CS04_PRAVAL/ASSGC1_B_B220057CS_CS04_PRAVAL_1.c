#include<stdio.h>
#include<stdlib.h>
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

queue createQueue(queue q)
{
    q = (queue)malloc(sizeof(struct queue));
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

node top(queue q)
{
    return q->arr[q->front];
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
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

node inorderAndPreorderToTree(int inorder[], int preorder[], int n, int* preorderIndex, int left, int right)
{
    if(left > right)
    {
        return NULL;
    }
    int i;
    for(i = 0; i < n; i++)
    {
        if(inorder[i] == preorder[*preorderIndex])
        {
            break;
        }
    }
    (*preorderIndex)++;
    node new_node = createNode(inorder[i]);
    new_node->left = inorderAndPreorderToTree(inorder, preorder, n, preorderIndex, left, i - 1);
    new_node->right = inorderAndPreorderToTree(inorder, preorder, n, preorderIndex, i + 1, right);
    return new_node;
}

void postorderTraversal(node root)
{
    if(root == NULL)
    {
        return;
    }
    postorderTraversal(root->left);
    postorderTraversal(root->right);
    printf("%d ", root->val);
}

void zigzagTraversal(node root)
{
    if(root == NULL)
    {
        printf("-1\n");
        return;
    }
    queue q = createQueue(q);
    q = push(q, root);
    int flag = 1;
    while(!queueEmpty(q))
    {
        int arr[100];
        int j = 0;
        int n = q->last - q->front;
        for(int i = 0; i < n; i++)
        {
            node temp_node = top(q);
            arr[j++] = temp_node->val;
            q = pop(q);
            if(temp_node->left != NULL)
                q = push(q, temp_node->left);
            if(temp_node->right != NULL)
                q = push(q, temp_node->right);
        }
        if(flag == 0)
            flag = 1;
        else
            flag = 0;
        if(flag == 1)
        {
            for(int i = 0; i < j; i++)
            {
                printf("%d ", arr[j - i - 1]);
            }
        }
        else
        {
            for(int i = 0; i < j; i++)
            {
                printf("%d ", arr[i]);
            }
        }
    }
    printf("\n");
}

int max(int num1, int num2) 
{
    return (num1 > num2 ) ? num1 : num2;
}

void levelMax(node root)
{
    if(root == NULL)
    {
        printf("-1\n");
        return;
    }
    queue q = createQueue(q);
    q = push(q, root);
    while(!queueEmpty(q))
    {
        int max = INT_MIN;
        int j = 0;
        int n = q->last - q->front;
        for(int i = 0; i < n; i++)
        {
            node temp_node = top(q);
            if(max < temp_node->val)
            {
                max = temp_node->val;
            }
            q = pop(q);
            if(temp_node->left != NULL)
                q = push(q, temp_node->left);
            if(temp_node->right != NULL)
                q = push(q, temp_node->right);
        }
        printf("%d ", max);
    }
    printf("\n");
}

// int diameterOfTree(node root, int* dia)
// {
//     if(root == NULL)
//     {
//         return -1;
//     }
//     int left = diameterOfTree(root->left, dia);
//     int right = diameterOfTree(root->right, dia);
//     *dia = max(*dia, left + right);
//     return max(left, right) + 1;
// }

int heightOfTree(node root)
{
    if(root == NULL)
    {
        return 0;
    }
    int left = heightOfTree(root->left);
    int right = heightOfTree(root->right);
    return max(left, right) + 1;
}

int diameterOfTree(node root)
{
    if(root == NULL)
    {
        return -1;
    }
    int left = heightOfTree(root->left);
    int right = heightOfTree(root->right);
    int left_dia = diameterOfTree(root->left);
    int right_dia = diameterOfTree(root->right);
    return max(max(left_dia, right_dia), left + right + 1);
}

void traverseSum(node root, int* sum, int* flag)
{
    if(root == NULL)
    {
        return;
    }
    if(root->left == NULL && root->right == NULL && *flag == 1)
    {
        *sum += root->val;
    }
    *flag = 1;
    traverseSum(root->left, sum, flag);
    *flag = 0;
    traverseSum(root->right, sum, flag);
}

int leftLeafSum(node root)
{
    int sum = 0;
    int flag = 0;
    traverseSum(root, &sum, &flag);
    return sum;
}

int main()
{
    int n;
    int* inorder = malloc(1000000 * sizeof(int));
    int* preorder = malloc(1000000 * sizeof(int));
    int preIndex = 0;
    int dia = 0;
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
    {
        scanf("%d", &inorder[i]);
    }
    for(int i = 0; i < n; i++)
    {
        scanf("%d", &preorder[i]);
    }
    node root = inorderAndPreorderToTree(inorder, preorder, n, &preIndex, 0, n - 1);
    while(1)
    {
        char ch;
        scanf("%c", &ch);
        if(ch == 'p')
        {
            postorderTraversal(root);
            printf("\n");
        }
        else if(ch == 'z')
        {
            zigzagTraversal(root);
        }
        else if(ch == 'm')
        {
            levelMax(root);
        }
        else if(ch == 'd')
        {   
            printf("%d\n", diameterOfTree(root));
        }
        else if(ch == 's')
        {
            printf("%d\n", leftLeafSum(root));
        }
        else if(ch == 'e')
        {
            break;
        }   
    }
    return 1;
}