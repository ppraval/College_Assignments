#include<stdio.h>
#include<stdlib.h>


int comparator(const void* p1, const void* p2) 
{ 
    const int(*x)[3] = p1; 
    const int(*y)[3] = p2;
    return (*x)[0] - (*y)[0]; 
}

void createEdgeList(int vertices, int edge_list[][3], int adj_list[][vertices + 1], int weight_list[][vertices + 1], int* k)
{
    for(int i = 0; i < vertices; i++)
    {
        for(int j = 1; adj_list[i][j] != -1; j++)
        {
            edge_list[(*k)][0] = weight_list[i][j];
            edge_list[(*k)][1] = adj_list[i][j];
            edge_list[(*k)][2] = i;
            (*k)++;
        }
    }
}

void makeSet(int vertices, int parent[], int rank[])
{
    for(int i = 0; i < vertices; i++)
    {
        parent[i] = i;
        rank[i] = 0;
    }
}

int findParent(int parent[], int component)
{
    if(parent[component] == component)
    {
        return parent[component];
    }
    return parent[component] = findParent(parent, parent[component]);
}

void unionSet(int vertices, int u, int v, int parent[], int rank[])
{
    u = findParent(parent, u);
    v = findParent(parent, v);

    if(rank[u] < rank[v])
    {
        parent[u] = v;
    }
    else if (rank[u] > rank[v])
    {
        parent[v] = u;
    }
    else
    {
        parent[v] = u;
        rank[u]++;  
    }
}

void kruskalMST(int vertices, int edge_list[][3], int k)
{
    qsort(edge_list, k, sizeof(edge_list[0]), comparator);
    int parent[vertices];
    int rank[vertices];

    makeSet(vertices, parent, rank);

    int min_weight = 0;
    for(int i = 0; i < k; i++)
    {
        int u = findParent(parent, edge_list[i][1]);
        int v = findParent(parent, edge_list[i][2]);
        int weight = edge_list[i][0];

        if(u != v)
        {
            unionSet(vertices, u, v, parent, rank);
            min_weight += weight;
        }
    }

    printf("%d\n", min_weight);
}

void printEdgeList(int edge_list[][3], int k)
{
    // qsort(edge_list, k, sizeof(edge_list[0]), comparator);
    printf("The edge list is = \n");
    for(int i = 0; i < k; i++)
    {
        printf("%d %d %d\n", edge_list[i][0], edge_list[i][1], edge_list[i][2]);
    }
}

int main()
{
    int vertices;
    scanf("%d", &vertices);
    int adj_list[vertices][vertices + 1];
    int weight_list[vertices][vertices + 1];
    int edge_list[100][vertices];
    int k = 0;
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
        weight_list[vertex][j++] = vertex;
        if(ch == '\n')
        {
            weight_list[vertex][j++] = -1;
            j = 0;
        }
        else
        {
            while(scanf("%d", &num) == 1)
            {
                ch = getchar();
                weight_list[vertex][j++] = num;
                if(ch == '\n')
                {
                    adj_list[vertex][j++] = -1;
                    j = 0;
                    break;
                }
            }
        }
    }
    createEdgeList(vertices, edge_list, adj_list, weight_list, &k);
    // printEdgeList(edge_list, k);
    kruskalMST(vertices, edge_list, k);
    return 0;
}