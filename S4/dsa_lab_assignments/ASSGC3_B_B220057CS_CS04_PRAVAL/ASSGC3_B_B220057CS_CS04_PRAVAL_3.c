#include<stdio.h>
#include<stdlib.h>

void listToMatrix(int vertices, int adj_list[][vertices + 1], int weight_list[][vertices + 1], int adj_matrix[][vertices])
{
    for(int i = 0; i < vertices; i++)
    {
        for(int j = 0; j < vertices; j++)
        {
            adj_matrix[i][j] = 0;
        }
    }

    for(int i = 0; i < vertices; i++)
    {
        for(int j = 1; adj_list[i][j] != -1; j++)
        {
            adj_matrix[i][adj_list[i][j]] = weight_list[i][j];
        }
    }

    // printf("The final matrix is \n");
    
    // for(int i = 0; i < vertices; i++)
    // {
    //     for(int j = 0; j < vertices; j++)
    //     {
    //         printf("%d ", adj_matrix[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("---------------\n");
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

int minDistance(int vertices, int distance[], int shortestPathSet[])
{
    int min = 9999, min_index;

    for(int v = 0; v < vertices; v++)
    {
        if(shortestPathSet[v] == 0 && distance[v] <= min)
        {
            min = distance[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijsktraShortestPaths(int vertices, int adj_matrix[][vertices], int start)
{
    int distance[vertices];
    int shortestPathSet[vertices];

    for(int i = 0; i < vertices; i++)
    {
        distance[i] = 9999;
        shortestPathSet[i] = 0;
    }

    distance[start] = 0;

    for(int count = 0; count < vertices - 1; count++)
    {
        int u = minDistance(vertices, distance, shortestPathSet);
        shortestPathSet[u] = 1;
        for(int v = 0; v < vertices; v++)
        {
            if(!shortestPathSet[v] && adj_matrix[u][v] && distance[u] != 9999 && distance[u] + adj_matrix[u][v] < distance[v])
            {
                distance[v] = distance[u] + adj_matrix[u][v];
            }
        }
    }

    for(int i = 0; i < vertices; i++)
    {
        printf("%d ", distance[i]);
    }
    printf("\n");
}

int main()
{
    int vertices;
    scanf("%d", &vertices);
    int adj_list[vertices][vertices + 1];
    int weight_list[vertices][vertices + 1];
    int adj_matrix[vertices][vertices];
    int source;
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
    // printf("The adj list is =\n");
    // printAdjList(vertices, adj_list);
    
    t = vertices;
    while(t--)
    {
        int num;
        int vertex;
        int j = 0;
        char ch;
        scanf("%d", &vertex);
        ch = getchar();
        weight_list[vertex - 1][j++] = vertex - 1;
        if(ch == '\n')
        {
            weight_list[vertex - 1][j++] = -1;
            j = 0;
        }
        else
        {
            while(scanf("%d", &num) == 1)
            {
                ch = getchar();
                weight_list[vertex - 1][j++] = num;
                if(ch == '\n')
                {
                    adj_list[vertex - 1][j++] = -1;
                    j = 0;
                    break;
                }
            }
        }
    }
    // printf("The weight list is =\n");
    // printAdjList(vertices, weight_list);
    listToMatrix(vertices, adj_list, weight_list, adj_matrix);
    scanf("%d", &source);
    dijsktraShortestPaths(vertices, adj_matrix, source - 1);
    return 0;
}