#include<stdio.h>
#include<stdlib.h>

struct node
{
    int val;
    struct node* next;
};

typedef struct node* node;

struct hash_table
{
    int max_size;
    node arr;
};

typedef struct hash_table* hash_table;

node createNode(int val)
{
    node new_node = (node)malloc(sizeof(struct node));
    new_node->val = val;
    new_node->next = NULL;
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
        table->arr[i].val = -1;
    }
    return table;
}

int hash(int val, int max_size)
{
    return val % max_size;
}

hash_table insert(hash_table table, int val)
{
    if(table == NULL)
    {
        printf("-1\n");
        return table;
    }
    int i = hash(val, table->max_size);
    if(table->arr[i].next == NULL)
    {
        node temp = createNode(val);
        table->arr[i].next = temp;
        return table;
    }
    node traverse_node = table->arr[i].next;
    node prev_node = NULL;
    while(traverse_node != NULL)
    {
        if(traverse_node->val > val)
        {
            break;
        }
        if(traverse_node->val == val)
        {
            printf("-1\n");
            return table;
        }
        prev_node = traverse_node;
        traverse_node = traverse_node->next;
    }
    node temp = createNode(val);
    prev_node->next = temp;
    temp->next = traverse_node;
    return table;
}

void search(hash_table table, int val)
{
    if(table == NULL)
    {
        printf("-1\n");
        return;
    }
    int index = hash(val, table->max_size);
    int i = 1;
    node traverse_node = table->arr[index].next;
    while(traverse_node != NULL)
    {
        if(traverse_node->val == val)
        {
            printf("%d %d\n", index, i);
            return;
        }
        i++;
        traverse_node = traverse_node->next;
    }
    printf("-1\n");
    return;
}

int isPresent(hash_table table, int val)
{
    if(table == NULL)
    {
        return 0;
    }
    int index = hash(val, table->max_size);
    int i = 1;
    node traverse_node = table->arr[index].next;
    while(traverse_node != NULL)
    {
        if(traverse_node->val == val)
        {
            printf("%d %d\n", index, i);
            return 1;
        }
        i++;
        traverse_node = traverse_node->next;
    }
    return 0;
}

int inTable(hash_table table, int val)
{
    if(table == NULL)
    {
        return 0;
    }
    int index = hash(val, table->max_size);
    node traverse_node = table->arr[index].next;
    while(traverse_node != NULL)
    {
        if(traverse_node->val == val)
        {
            return 1;
        }
        traverse_node = traverse_node->next;
    }
    return 0;
}

hash_table delete(hash_table table, int val)
{
    if(table == NULL || isPresent(table, val) == 0)
    {
        printf("-1\n");
        return table;
    }
    int index = hash(val, table->max_size);
    node traverse_node = table->arr[index].next;
    node prev_node = NULL;
    while(traverse_node != NULL)
    {
        if(traverse_node->val == val)
        {
            break;
        }
        prev_node = traverse_node;
        traverse_node = traverse_node->next;
    }
    if(prev_node == NULL)
    {
        table->arr[index].next = traverse_node->next;
    }
    else
    {
        prev_node->next = traverse_node->next;
        traverse_node->next = NULL;
    }
    return table;
}

hash_table update(hash_table table, int old_val, int new_val)
{
    if(table == NULL)
    {
        printf("-1\n");
        return table;
    }
    if(inTable(table, old_val) == 0 || inTable(table, new_val) == 1)
    {
        printf("-1\n");
        return table;
    }
    table = delete(table, old_val);
    table = insert(table, new_val);
    return table;
}

void printElementsInChain(hash_table table, int index)
{
    if(table->arr[index].next != NULL)
    {
        node traverse_node = table->arr[index].next;
        while(traverse_node != NULL)
        {
            printf("%d ",  traverse_node->val);
            traverse_node = traverse_node->next;
        }
        printf("\n");
        return;
    }
    printf("-1\n");
}

int main()
{
    int size;
    scanf("%d", &size);
    hash_table table = createHashTable(size);
    while(1)
    {
        char op;
        int val_1, val_2;
        scanf("%c", &op);
        if(op == 'i')
        {
            scanf(" %d", &val_1);
            table = insert(table, val_1);
        }
        else if(op == 'd')
        {
            scanf(" %d", &val_1);
            table = delete(table, val_1);
        }
        else if(op == 'u')
        {
            scanf(" %d %d", &val_1, &val_2);
            table = update(table, val_1, val_2);
        }
        else if(op == 's')
        {
            scanf(" %d", &val_1);
            search(table, val_1);
        }
        else if(op == 'p')
        {
            scanf(" %d", &val_1);
            printElementsInChain(table, val_1);
        }
        else if(op == 'e')
        {
            break;
        }
    }
    return 0;
}