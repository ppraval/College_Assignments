#include<stdio.h>
#include<stdlib.h>

int min(int a, int b)
{
    if(a < b)
        return a;
    return b;
}

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

void dfs(int vertices, int adj_list[][vertices + 1], int visited[], int v, int* count)
{
    visited[v] = 1;
    for(int i = 0; adj_list[v][i] != -1; i++)
    {
        if(visited[adj_list[v][i]] == 0)
        {
            (*count)++;
            dfs(vertices, adj_list, visited, adj_list[v][i], count);
        }
    }
}

int numberOfComponents(int vertices, int adj_list[][vertices + 1])
{
    int visited[vertices];
    for(int i = 0; i < vertices; i++)
    {
        visited[i] = 0;
    }
    int num = 0;
    int count = 0;
    for(int i = 0; i < vertices; i++)
    {
        if(visited[i] == 0)
        {
            dfs(vertices, adj_list, visited, i, &count);
            num++;
        }
    }
    return num;
}

void sizeOfComponents(int vertices, int adj_list[][vertices + 1])
{
    int visited[vertices];
    for(int i = 0; i < vertices; i++)
    {
        visited[i] = 0;
    }
    int arr[100];
    int n = 0;
    for(int i = 0; i < vertices; i++)
    {
        int count = 1;
        if(visited[i] == 0)
        {
            dfs(vertices, adj_list, visited, i, &count);
            arr[n++] = count;
        }
    }
    int key;
    for (int j = 1; j < n; j++)
    {  
        key = arr[j];
        int i = j - 1;
        while (i >= 0 && arr[i] >= key)
        {
            arr[i + 1] = arr[i];
            i = --i; 
        }
        arr[i + 1] = key;
    }
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void apdfs(int vertices, int adj_list[][vertices + 1], int visited[], int v, int deleted)
{
    visited[v] = 1;
    for(int i = 0; adj_list[v][i] != -1; i++)
    {
        if(adj_list[v][i] != deleted)
        {
            if(visited[adj_list[v][i]] == 0)
            {
                apdfs(vertices, adj_list, visited, adj_list[v][i], deleted);
            }
        }
    }
}

int numberOfArticulatedPoints(int vertices, int adj_list[][vertices + 1], int x)
{
    int count = 0;
    for(int i = 0; i < vertices; i++)
    {
        int visited[vertices];
        for(int j = 0; j < vertices; j++)
        {
            visited[j] = 0;
        }
        int components = 0;
        for(int j = 0; j < vertices; j++)
        {
            if(i != j)
            {
                if(visited[j] == 0)
                {
                    components++;
                    apdfs(vertices, adj_list, visited, j, i);
                }        
            }
        }
        if(components > x)
        {
            count++;
        }        
    }
    if(count == 0)
        return -1;
    return count;
}

void bridgedfs(int vertices, int v, int adj_list[][vertices + 1], int visited[], int disc[], int low[], int parent, int* time, int* count)
{
    visited[v] = 1;
    disc[v] = low[v] = ++(*time);
    for(int i = 0; adj_list[v][i] != -1; i++)
    {
        int u = adj_list[v][i];
        if(u == parent)
        {
            continue;
        }
        if(visited[u] == 1)
        {
            low[v] = min(low[v], disc[u]);
        }
        else
        {
            parent = v;
            bridgedfs(vertices, u, adj_list, visited, disc, low, parent, time, count);
            low[v] = min(low[v], low[u]);
        }
        if(low[u] > disc[v])
        {
            (*count)++;
        }         
    }
}

int numberOfBridges(int vertices, int adj_list[][vertices + 1])
{
    int visited[vertices];
    int disc[vertices];
    int low[vertices];
    for(int i = 0; i < vertices; i++)
    {
        visited[i] = 0;
        disc[i] = -1;
        low[i] = -1;
    }
    int parent = -1;
    int time = 0;
    int count = 0;

    for(int i = 0; i < vertices; i++)
    {
        if(visited[i] == 0)
        {
            bridgedfs(vertices, i, adj_list, visited, disc, low, parent, &time, &count);
        }
    }
    if(count == 0)
    {
        return -1;
    }
    else
    {
        return count;
    }
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
        adj_list[vertex - 1][j++] = vertex - 1;
        if(ch == '\n')
        {
            adj_list[vertex - 1][j++] = -1;
            j = 0;
        }
        else
        {
            while(scanf("%d", &num) == 1)
            {
                ch = getchar();
                adj_list[vertex - 1][j++] = num - 1;
                if(ch == '\n')
                {
                    adj_list[vertex - 1][j++] = -1;
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
        if(ch == 'n')
        {
            printf("%d\n", numberOfComponents(vertices, adj_list));
        }
        else if (ch == 's')
        {
            sizeOfComponents(vertices, adj_list);
        }
        else if (ch == 'a')
        {
            int x = numberOfComponents(vertices, adj_list);
            printf("%d\n", numberOfArticulatedPoints(vertices, adj_list, x));
        }
        else if (ch == 'b')
        {
            // int x = numberOfComponents(vertices, adj_list);
            printf("%d\n", numberOfBridges(vertices, adj_list));
        }
        else if (ch == 't')
        {
            break;
        }
    }
    return 0;
}