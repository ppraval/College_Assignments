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

    void inorderTraversal(node root)
    {
        if(root == NULL)
        {
            return;
        }
        inorderTraversal(root->left);
        printf("%d ", root->val);
        inorderTraversal(root->right);
    }

    void levelOrderTraversal(node root)
    {
        if(root == NULL)
        {
            printf("-1\n");
            return;
        }
        queue q = createQueue();
        q = push(q, root);
        while(!queueEmpty(q))
        {
            int arr[100];
            int j = 0;
            int n = q->last - q->front;
            for(int i = 0; i < n; i++)
            {
                node temp = queueFront(q);
                arr[j++] = temp->val;
                q = pop(q);
                if(temp->left != NULL)
                    q = push(q, temp->left);            
                if(temp->right != NULL)
                    q = push(q, temp->right);
            }
            for(int i = 0; i < j; i++)
            {
                printf("%d ", arr[i]);
            }
        }
        printf("\n");
    }

    void rightViewOfTree(node root)
    {
        if(root == NULL)
        {
            printf("-1\n");
            return;
        }
        queue q = createQueue();
        q = push(q, root);
        while(!queueEmpty(q))
        {
            int j = 0;
            int n = q->last - q->front;
            for(int i = 0; i < n; i++)
            {
                node temp = queueFront(q);
                if(i == n - 1)
                    printf("%d ", temp->val);
                q = pop(q);
                if(temp->left != NULL)
                    q = push(q, temp->left);            
                if(temp->right != NULL)
                    q = push(q, temp->right);
            }
        }
        printf("\n");
    }

    int max(int a, int b)
    {
        if(a >= b)
            return a;
        return b;
    }

    int min(int a, int b)
    {
        if(a <= b)
            return a;
        return b;
    }

    int summingUpBst(node root, int* max_val, int* min_val, int* max_sum)
    {
        if(root == NULL)
        {
            return 0;
        }

        int left_max = INT_MIN, left_min = INT_MAX;
        int right_max = INT_MIN, right_min = INT_MAX;

        int left_sum = summingUpBst(root->left, &left_max, &left_min, max_sum);
        int right_sum = summingUpBst(root->right, &right_max, &right_min, max_sum);

        int val = root->val + left_sum + right_sum; 
        
        if(left_max < root->val && right_min > root->val)
        {
            *max_sum = max(*max_sum, val);
            *max_val = max(root->val, right_max);
            *min_val = min(root->val, left_min);
        }
        else
        {
            *max_val = INT_MAX;
            *min_val = INT_MIN;
        }
        return val;
    }

    int maximumSumBST(node root)
    {
        if(root == NULL)
        {
            printf("-1\n");
            return 0;
        }
        int max_sum = 0;
        int max_val = INT_MAX;
        int min_val = INT_MIN;
        summingUpBst(root, &max_val, &min_val, &max_sum);
        printf("%d\n", max_sum);
        return max_sum;
    }

    int main()
    {
        char* arr = (char*)malloc(1000000 * sizeof(char));
        int i = 0;
        scanf("%[^\n]s", arr);
        node root = NULL;
        root = createTreeFromBrackets(arr, &i);
        while(1)
        {
            char ch;
            scanf("%c", &ch);
            if(ch == 'l')
            {
                levelOrderTraversal(root);
            }
            else if(ch == 'r')
            {
                rightViewOfTree(root);
            }
            else if(ch == 'm')
            {
                maximumSumBST(root);
            }
            else if(ch == 'e')
            {
                break;
            }
        }
        return 1;
    }