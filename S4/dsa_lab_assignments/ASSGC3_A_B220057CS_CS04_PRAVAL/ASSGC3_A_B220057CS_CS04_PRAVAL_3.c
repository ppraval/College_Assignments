#include<stdio.h>
#include<stdlib.h>

void printAdjList(int vertices, int adj_list[][vertices + 1])
{
    for(int i = 0; i < vertices; i++)
    {
        for(int j = 0; adj_list[i][j] != -1; j++)
        {
            printf("%d ", adj_list[i][j]);
        }
        printf("\n");
    }
}

// Have to add the part that prints -1 when there are no path between the graph nodes
void dfs(int vertices, int adj_list[][vertices + 1], int visited[], int v, int end_node, int path[], int* j, int* flag)
{
    visited[v] = 1;
    path[(*j)++] = v;
    if(v == end_node)
    {
        *flag = 1;
        for(int i = 0; i < *j; i++)
        {
            printf("%d ", path[i]);
        }
        printf("\n");
    }
    else
    {
        for(int i = 0; adj_list[v][i] != -1; i++)
        {
            if(visited[adj_list[v][i]] == 0)
            {
                dfs(vertices, adj_list, visited, adj_list[v][i], end_node, path, j, flag);
            }
        }
    }
    (*j)--;
    visited[v] = 0;
}

void printPath(int vertices, int adj_list[][vertices + 1], int start_node, int end_node)
{
    int visited[vertices];
    for(int i = 0; i < vertices; i++)
    {
        visited[i] = 0;
    }
    int path[100];
    int j = 0;
    int flag = 0;
    dfs(vertices, adj_list, visited, start_node, end_node, path, &j, &flag);
    if(flag == 0)
    {
        printf("-1\n");
    }
}

int isCyclic(int vertices, int adj_list[][vertices + 1], int visited[], int v, int parent)
{
    visited[v] = 1;
    for(int i = 1; adj_list[v][i] != -1; i++)
    {
        if(visited[adj_list[v][i]] == 0)
        {
            if(isCyclic(vertices, adj_list, visited, adj_list[v][i], v))
            {
                return 1;
            }
        }
        else if(adj_list[v][i] != parent)
        {
            return 1;
        }
    }
    return 0;
}

void isTree(int vertices, int adj_list[][vertices + 1])
{
    int visited[vertices];
    for(int i = 0; i < vertices; i++)
    {
        visited[i] = 0;
    } 
    if(isCyclic(vertices, adj_list, visited, 0, -1))
    {
        printf("-1\n");
        return;
    }
    for(int i = 0; i < vertices; i++)
    {
        if(visited[i] == 0)
        {
            printf("-1\n");
            return;
        }
    }
    printf("1\n");
    return;
}

int main()
{
    int vertices;
    scanf("%d", &vertices);
    int adj_list[vertices][vertices + 1];
    int t = vertices;
    while(t--)
    {
        int num;
        int vertex;
        int j = 0;
        char ch;
        scanf("%d", &vertex);
        ch = getchar();
        adj_list[vertex][j++] = vertex;
        if(ch == '\n')
        {
            adj_list[vertex][j++] = -1;
            j = 0;
        }
        else
        {
            while(scanf("%d", &num) == 1)
            {
                ch = getchar();
                adj_list[vertex][j++] = num;
                if(ch == '\n')
                {
                    adj_list[vertex][j++] = -1;
                    j = 0;
                    break;
                }
            }
        }
    }

    while(1)
    {
        char ch;
        int start_node, end_node;
        scanf("%c", &ch);
        if(ch == 'a')
        {
            scanf(" %d %d", &start_node, &end_node);
            if(start_node == end_node)
            {
                printf("-1\n");
            }
            else
            {
                printPath(vertices, adj_list, start_node, end_node);
            }
        }
        else if (ch == 't')
        {
            isTree(vertices, adj_list);
        }
        else if (ch == 'x')
        {
            break;
        }
    }
    return 0;
}