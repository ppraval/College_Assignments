#include<stdio.h>
#include<stdlib.h>

struct queue
{
    int arr[1000];
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

queue push(queue q, int n)
{
    q->arr[q->last++] = n;
    return q;
}

queue pop(queue q)
{
    q->front++;
    return q;
}

int front(queue q)
{
    return q->arr[q->front];
}

int queueEmpty(queue q)
{
    if(q->front == q->last)
        return 1;
    return 0;
}

struct stack
{
    int arr[1000];
    int top;
};

typedef struct stack* stack;

stack createStack(stack s)
{
    s = (stack)malloc(sizeof(struct stack));
    s->top = -1;
    return s;
}

stack s_push(stack s, int ch)
{
    s->top++;
    s->arr[s->top] = ch;
    return s;
}

stack s_pop(stack s)
{
    s->top--;
    return s;
}

int stackEmpty(stack s)
{
    if(s->top == -1)
        return 1;
    return 0;
}

int top(stack s)
{
    if(!stackEmpty(s))
        return s->arr[s->top];
    return -1;
}

void printAdjMatrix(int vertices, int adj_matrix[][vertices])
{
    for(int i = 0; i < vertices; i++)
    {
        for(int j = 0; j < vertices; j++)
        {
            printf("%d ", adj_matrix[i][j]);
        }
        printf("\n");
    }
}

void bfs(int vertices, int adj_matrix[][vertices])
{
    queue q = createQueue(q);
    int in_degree[vertices];
    for(int i = 0; i < vertices; i++)
    {
        in_degree[i] = 0;
    }
    int visited = 0;
    for(int i = 0; i < vertices; i++)
    {
        for(int j = 0; j < vertices; j++)
        {
            if(adj_matrix[i][j] == 1)
            {
                in_degree[j]++;
            }
        }
    }
    for(int i = 0; i < vertices; i++)
    {
        if(in_degree[i] == 0)
        {
            q = push(q, i);
        }
    }
    while(!queueEmpty(q))
    {
        int current = front(q);
        q = pop(q);
        visited++;
        for(int i = 0; i < vertices; i++)
        {
            if(adj_matrix[current][i] == 1)
            {
                in_degree[i]--;
                if(in_degree[i] == 0)
                    q = push(q, i);
            }
        }
    }
    if(visited == vertices)
    {
        printf("1\n");
    }
    else
    {
        printf("-1\n");
    }
}

void filldfs(int vertices, int adj_matrix[][vertices], int visited[], int v, stack* s)
{
    visited[v] = 1;
    for(int i = 0; i < vertices; i++)
    {
        if(adj_matrix[v][i] == 1)
        {
            if(visited[i] == 0)
                filldfs(vertices, adj_matrix, visited, i, s);
        }
    }
    *s = s_push(*s, v);
}

void dfs(int vertices, int adj_matrix[][vertices], int visited[], int v)
{
    visited[v] = 1;
    // cout << v << " ";
    for(int i = 0; i < vertices; i++)
    {
        if(adj_matrix[v][i] == 1)
        {
            if(visited[i] == 0)
                dfs(vertices, adj_matrix, visited, i);
        }
    }
}

void kosarajunumberofsccs(int vertices, int adj_matrix[][vertices])
{
    stack s = createStack(s);
    int visited[vertices];
    for(int i = 0; i < vertices; i++)
    {
        visited[i] = 0;
    }

    int count = 0;
    for(int i = 0; i < vertices; i++)
    {
        if(visited[i] == 0)
        {
            filldfs(vertices, adj_matrix, visited, i, &s);
        }
    }

    int adj_matrix_transpose[vertices][vertices];
    for(int i = 0; i < vertices; i++)
    {
        for(int j = 0; j < vertices; j++)
        {
            adj_matrix_transpose[i][j] = adj_matrix[j][i];
        }
    }

    for(int i = 0; i < vertices; i++)
    {
        visited[i] = 0;
    }

    while(!stackEmpty(s))
    {
        int num = top(s);
        s = s_pop(s);
        if(visited[num] == 0)
        {
            dfs(vertices, adj_matrix_transpose, visited, num);
            count++;
        }
    }
    printf("%d\n", count);
    return;
}

int main()
{
    int vertices;
    scanf("%d", &vertices);
    int adj_matrix[vertices][vertices];
    for(int i = 0; i < vertices; i++)
    {
        for(int j = 0; j < vertices; j++)
        {
            scanf("%d", &adj_matrix[i][j]);
        }
    }
    while(1)
    {
        char ch;
        scanf("%c", &ch);
        if(ch == 't')
        {
            bfs(vertices, adj_matrix);
        }
        else if (ch == 'c')
        {
            kosarajunumberofsccs(vertices, adj_matrix);
        }
        else if(ch == 'x')
        {
            break;
        }
    }
    return 0;
}