#include<stdio.h>
#include<stdlib.h>

void printSequence(int vertices, int parent[], int adj_matrix[][vertices], int path[], int n)
{
    for(int i = 0; i < n; i++)
    {
        if(parent[path[i]] == -1)
        {
            continue;
        }
        printf("%d %d ", parent[path[i]], path[i]);
    }
    printf("\n");
}

void weightOfMst(int vertices, int parent[], int adj_matrix[][vertices])
{
    int sum = 0;
    for(int i = 0; i < vertices; i++)
    {
        if(parent[i] == -1)
        {
            continue;
        }
        sum += adj_matrix[i][parent[i]];
    }
    printf("%d\n", sum);
}

int minKey(int vertices, int key[], int curr_mst[], int path[], int* j)
{
    int min = 9999;
    int min_index;
    for(int v = 0; v < vertices; v++)
    {
        if(curr_mst[v] == 0 && key[v] < min)
        {
            min = key[v];
            min_index = v;
        }
    }
    path[(*j)++] = min_index;
    return min_index;
}

void primMst(int vertices, int adj_matrix[][vertices], int vertex, int flag)
{
    int parent[vertices];
    int key[vertices];
    int curr_mst[vertices];
    int path[100];
    int j = 0;

    for(int i = 0; i < vertices; i++)
    {
        key[i] = 9999;
        curr_mst[i] = 0;
    }
    key[vertex] = 0;
    parent[vertex] = -1;

    for(int count = 0; count < vertices; count++)
    {
        int u = minKey(vertices, key, curr_mst, path, &j);
        curr_mst[u] = 1;
        for(int v = 0; v < vertices; v++)
        {
            if(adj_matrix[u][v] && curr_mst[v] == 0 && adj_matrix[u][v] < key[v])
            {
                parent[v] = u;
                key[v] = adj_matrix[u][v];
            }
        }
    }
    if(flag == 1)
    {
        printSequence(vertices, parent, adj_matrix, path, j);
    }
    else
    {
        weightOfMst(vertices, parent, adj_matrix);
    }
}

int main()
{
    int vertices;
    scanf("%d", &vertices);
    int adj_matrix[vertices][vertices];
    int flag = 0;
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
        int vertex;
        scanf("%c", &ch);
        if(ch == 't')
        {
            primMst(vertices, adj_matrix, 0, 0);
        }
        else if(ch == 's')
        {
            char c = getchar();
            scanf("%d", &vertex);
            primMst(vertices, adj_matrix, vertex, 1);
        }
        else if(ch == 'x')
        {
            break;
        }
    }
    return 0;
}