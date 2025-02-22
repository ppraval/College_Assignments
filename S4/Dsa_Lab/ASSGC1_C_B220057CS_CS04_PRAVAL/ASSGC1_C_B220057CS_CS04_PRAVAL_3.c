#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node
{
    char* name;
    int branch;
    struct node* next;
};

typedef struct node* node;

struct hash_table
{
    int max_size;
    node arr;
};

typedef struct hash_table* hash_table;

node createNode(char name[], int branch)
{
    node new_node = (node)malloc(sizeof(struct node));
    new_node->name = name;
    new_node->next = NULL;
    new_node->branch = branch;
    return new_node;
}

hash_table createHashTable(int max_size)
{
    hash_table table = malloc(sizeof(struct hash_table));
    table->arr = malloc(sizeof(struct node) * max_size);
    table->max_size = max_size;
    for(int i = 0; i < max_size; i++)
    {
        table->arr[i].next = NULL;
        table->arr[i].name = NULL;
        table->arr[i].branch = -1;
    }
    return table;
}

int hash(char str[], int age)
{
    int sum = 0;
    for(int i = 0; i < strlen(str); i++)
    {
        sum += str[i];
    }
    sum += age;
    return sum % 4;
}

hash_table insert(hash_table table, char name[], int age, int branch)
{
    if(table == NULL)
    {
        printf("-1\n");
        return table;
    }
    int i = hash(name, age);
    if(table->arr[i].next == NULL)
    {
        node temp = createNode(name, branch);
        table->arr[i].next = temp;
        return table;
    }
    node traverse_node = table->arr[i].next;
    while(traverse_node->next != NULL)
    {
        traverse_node = traverse_node->next;
    }
    node temp = createNode(name, branch);
    traverse_node->next = temp;
    return table;
}

int count(hash_table table, int index)
{
    if(table == NULL)
    {
        printf("-1\n");
        return 0;
    }
    node traverse_node = table->arr[index].next;
    int count = 0;
    while(traverse_node != NULL)
    {
        count++;
        traverse_node = traverse_node->next;
    }
    return count;
}

void printElementsInChain(hash_table table, int index)
{
    printf("%d ", count(table, index));
    if(table->arr[index].next != NULL)
    {
        node traverse_node = table->arr[index].next;
        while(traverse_node != NULL)
        {
            printf("%s ", traverse_node->name);
            traverse_node = traverse_node->next;
        }
    }
    printf("\n");
    return;
}

void printBranchSpecificInChain(hash_table table, int index, int branch)
{
    int flag = 0;
    if(table->arr[index].next != NULL)
    {
        node traverse_node = table->arr[index].next;
        while(traverse_node != NULL)
        {  
            if(traverse_node->branch == branch)
            {
                printf("%s ", traverse_node->name);
                flag = 1;
            }
            traverse_node = traverse_node->next;
        }
    }
    if(flag == 0)
        printf("-1");
    printf("\n");
    return;
}

int main()
{
    hash_table table = createHashTable(4);
    int t;
    scanf("%d", &t);
    while(t--)
    {
        char* input = malloc(sizeof(char) * 100);
        char* name = malloc(sizeof(char) * 100);
        char* roll = malloc(sizeof(char) * 9);
        int age;
        int branch;
        scanf(" %[^\n]", input);
        sscanf(input, "%s %s %d", name, roll, &age);
        if(roll[7] == 'C' && roll[8] == 'S')
        {
            branch = 0;
        }
        else if(roll[7] == 'E' && roll[8] == 'E')
        {
            branch = 1;
        }
        else if(roll[7] == 'E' && roll[8] == 'C')
        {
            branch = 2;
        }
        else if(roll[7] == 'C' && roll[8] == 'E')
        {
            branch = 3;
        }
        table = insert(table, name, age, branch);
    }
    while(1)
    {
        char op, b1, b2;
        int index, branch;
        scanf("%c", &op);
        if(op == 'c')
        {
            scanf(" %d", &index);
            printElementsInChain(table, index);
        }
        else if (op - '0' >= 0 && op - '0' <= 3)
        {
            index = op - '0';
            scanf(" %c%c", &b1, &b2);
            if((b1 == 'C' && b2 == 'S') || (b1 == 'c' && b2 == 's'))
            {
                branch = 0;
            }
            if((b1 == 'E' && b2 == 'E') || (b1 == 'e' && b2 == 'e'))
            {
                branch = 1;
            }
            if((b1 == 'E' && b2 == 'C') || (b1 == 'e' && b2 == 'c'))
            {
                branch = 2;
            }
            if((b1 == 'C' && b2 == 'E') || (b1 == 'c' && b2 == 'e'))
            {
                branch = 3;
            }
            printBranchSpecificInChain(table, index, branch);
        }
        else if(op == 'e')
        {
            break;
        }
        
    }
    return 0;
}