#include<stdio.h>
#include<stdlib.h>

void floydWarshall(int vertices, int adj_matrix[][vertices])
{
    int new_adj_matrix[vertices][vertices];
    for(int i = 0; i < vertices; i++)
    {
        for(int j = 0; j < vertices; j++)
        {
            new_adj_matrix[i][j] = adj_matrix[i][j];
        }
    }

    for(int k = 0; k < vertices; k++)
    {
        for(int i = 0; i < vertices; i++)
        {
            for(int j = 0; j < vertices; j++)
            {
                if(new_adj_matrix[i][k] != -1 && new_adj_matrix[k][j] != -1 && (new_adj_matrix[i][j] == -1 || new_adj_matrix[i][k] + new_adj_matrix[k][j] < new_adj_matrix[i][j]))
                {
                    new_adj_matrix[i][j] = new_adj_matrix[i][k] + new_adj_matrix[k][j];
                }
            }
        }
    }

    for(int i = 0; i < vertices; i++)
    {
        for(int j = 0; j < vertices; j++)
        {
            printf("%d ", new_adj_matrix[i][j]);
        }
        printf("\n");
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
    floydWarshall(vertices, adj_matrix);
    return 0;
}