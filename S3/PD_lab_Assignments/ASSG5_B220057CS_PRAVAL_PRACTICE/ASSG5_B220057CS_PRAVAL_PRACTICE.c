#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node_list 
{
    int pid;
    int bn;
    struct node_list *next;
};

typedef struct node_list *node;


node createNode(int pid, int bn)
{
    node new_node;
    new_node = (node) malloc(sizeof(struct node_list));
    new_node->pid = pid;
    new_node->bn = bn;
    new_node->next = NULL;
    return new_node;
}

// a
node addPatient(node head, int pid, int bn)
{
    node new_node, current_node, previous_node = NULL;
    new_node = createNode(pid, bn);

    if(head == NULL)
    {
        head = new_node;
        return head;
    }
    
    current_node = head;
    while(current_node != NULL && current_node->bn < bn) 
    {
        previous_node = current_node;
        current_node = current_node->next;
    }

    if(previous_node == NULL) 
    {
        new_node->next = head;
        head = new_node;
    } 
    else 
    {
        previous_node->next = new_node;
        new_node->next = current_node;
    }
    return head;
}

// c
node callPatient(node head)
{
    if(head == NULL)
    {
        printf("0\n");
        return head;
    }
    
    printf("%d\n", head->pid);
    node new_head = head->next;
    free(head);
    return new_head;
}

// n
void nextToken(node head)
{
    if(head == NULL)
    {
        printf("0\n");
    }
    else
    {
        printf("%d\n", head->pid);
    }
}

// bn
void getBN(node head, int pid)
{
    node current_node = head;
    while(current_node != NULL && current_node->pid != pid)
    {
        current_node = current_node->next;
    }
    
    if(current_node == NULL)
    {
        printf("0\n");
    }
    else
    {
        printf("%d\n", current_node->bn);
    }
}

// pid
void getPID(node head, int bn)
{
    node current_node = head;
    while(current_node != NULL && current_node->bn != bn)
    {
        current_node = current_node->next;
    }
    
    if(current_node == NULL)
    {
        printf("0\n");
    }
    else
    {
        printf("%d\n", current_node->pid);
    }
}

// dA & dB
void doctorPID(node head, int min, int max)
{
    int counter = 0;
    node current_node = head;
    while(current_node != NULL)
    {
        if(current_node->pid >= min && current_node->pid <= max)
        {
            if(counter > 0)
            {
                printf(" ");
            }
            printf("%d", current_node->pid);
            counter ++;
        }
        current_node = current_node->next;
    }
    
    if(counter == 0)
    {
        printf("0");
    }
    printf("\n");
}

// d
void display(node head)
{
    if(head == NULL) 
    {
        printf("0\n");
    }
    else 
    {
        node current_node = head;
        while(current_node != NULL)
        {
            printf("%d %d\n", current_node->pid, current_node->bn);
            current_node = current_node->next;
        }
    }
}

int main()
{
    int MAX = 100;
    char input[MAX];
    char word[MAX];
    int num1, num2;

    node head = NULL;

    while(1) {
        fgets(input, MAX - 1, stdin);
        sscanf(input, "%s %d %d", word, &num1, &num2);
        if (strcmp(word, "a") == 0) {
            head = addPatient(head, num1, num2);
            //display(head);
        } else if (strcmp(word, "c") == 0) {
            head = callPatient(head);
            //display(head);
        } else if (strcmp(word, "n") == 0) {
            nextToken(head);
        } else if (strcmp(word, "bn") == 0) {
            getBN(head, num1);
        } else if (strcmp(word, "pid") == 0) {
            getPID(head, num1);
        } else if (strcmp(word, "dA") == 0) {
            doctorPID(head, 1, 1000);
        } else if (strcmp(word, "dB") == 0) {
            doctorPID(head, 1001, 10000);
        } else if (strcmp(word, "d") == 0) {
            display(head);
        } else if (strcmp(word, "e") == 0) {
            break;
        }
    }
   return 1;
}