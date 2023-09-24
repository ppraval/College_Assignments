#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct linkedList {
    int key;
    struct linkedList *next;
    struct linkedList *prev;
};

typedef struct linkedList *node;

node createNode()
{
    node new_node;
    new_node = (node) malloc(sizeof(struct linkedList));
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

// f
node insertAthead(node head, int key)
{
    node new_head, new_node;
    new_node = createNode();
    new_node->key = key;
    if(head == NULL) {
        head = new_node;
    } else {
        new_node->next = head;
        head->prev = new_node;
        head = new_node;
    }
    return head;
}

// t
node insertAtEnd(node head, int key)
{
    node new_node, current_node;
    new_node = createNode();
    new_node->key = key;
    new_node->next = NULL;
    if (head == NULL) {
        head = new_node;
    } else {
        current_node = head;
        node previous_node = NULL;
        while(current_node->next != NULL) {
            previous_node = current_node;
            current_node = current_node->next;
        }
        new_node->prev = current_node;
        current_node->next = new_node;
    }
    return head;
}

// a
node insertAfterAnother(node head, int exist_key, int key)
{
    node new_node, current_node, next_node;
    new_node = createNode();
    new_node->key = key;
    current_node = head;
    while(current_node->key != exist_key) {
        current_node = current_node->next;
    }
    
    new_node->prev = current_node;
    new_node->next = current_node->next;
    current_node->next = new_node;
    if(new_node->next != NULL) {
        new_node->next->prev = new_node;
    }
    return head;
}

// b
node insertBeforeAnother(node head, int exist_key, int key)
{
    node new_node, current_node, previous_node = NULL;
    new_node = createNode();
    new_node->key = key;
    current_node = head;
    while(current_node->key != exist_key) {
        previous_node = current_node;
        current_node = current_node->next;
    }
    if(previous_node == NULL) {
        insertAthead(head, key);
    } else {
        previous_node->next = new_node;
        new_node->next = current_node;
        new_node->prev = previous_node;
        if (current_node != NULL)
        {
            current_node->prev = new_node;
        }
    }
    return head;
}

// i
node deleteheadNode(node head)
{
    if(head == NULL)
    {
        printf("-1\n");
        return head;
    }
    if(head->next == NULL )
    {
        return NULL;
    }
    
    printf("%d\n", head->key);
    node new_head = head->next;
    new_head->prev = NULL;
    free(head);
    return new_head;
}

// d
node deleteNode(node head, int key)
{
    if(head == NULL)
    {
        printf("-1\n");
        return head;
    }
    
    if(head->key == key) {
        return deleteheadNode(head);
    }
    
    node current_node = head, previous_node, next_node;
    while(current_node != NULL && current_node->key != key) {
        previous_node = current_node;
        current_node = current_node->next;
        if(current_node == NULL) {
            printf("-1\n");
            return head;
        }
        next_node = current_node->next;
    }
    
    previous_node->next = next_node;
    if(next_node != NULL)
        next_node->prev = previous_node;
    printf("%d\n", current_node->key);
    free(current_node);
    return head;
}

// l
node deleteLastnode(node head)
{
    if(head == NULL) {
        printf("-1\n");
        return head;
    } else if(head->next == NULL) {
        return deleteheadNode(head);
    } else {
        node current_node = head;
        node previous_node = NULL;
        while(current_node->next != NULL) {
            previous_node = current_node;
            current_node = current_node->next;
        }
        printf("%d\n", current_node->key);
        previous_node->next = NULL;
        free(current_node);
        return head;
    }
}

// ds - rev
void printListReverse(node head)
{
    if(head == NULL) {
        printf("List is empty\n");
    } else {
        node current_node = head;
        while(current_node->next != NULL) {
            current_node = current_node->next;
        }
        
        printf("REVERSE: ");
        while(current_node != NULL) {
            printf("%d ", current_node->key);
            current_node = current_node->prev;
        }
        printf("\n");   
    }
}

// ds
void printList(node head)
{
    if(head == NULL) 
    {
        printf("-1\n");
    }
    else 
    {
        node current_node = head;
        while(current_node != NULL) 
        {
            printf("%d", current_node->key);
            if(current_node->next != NULL)
                printf(" ");
            current_node = current_node->next;
        }
        printf("\n");
    }
}

// s
void searchNode(node head, int key)
{
    if(head == NULL) {
        printf("-1\n");
        return;
    }
    node current_node = head;
    while (current_node != NULL && current_node->key != key) {
        current_node = current_node->next;
    }
    if(current_node == NULL) {
        printf("-1\n");
    } else {
        printf("1\n");
    }
}

// r
node reverseList(node head)
{
    if(head == NULL || head->next == NULL) {
        return head;
    }

    node current_node = head;
    node previous_node = NULL;
    node new_head = head;
    node temp_node;
    while(current_node != NULL) {
        temp_node = current_node->next;
        new_head = current_node;
        new_head->next = previous_node;
        previous_node = current_node;
        current_node = temp_node;
    }
    return new_head;
}

node reverseNegative(node head)
{
    node forward = head;
    node negative = NULL;
    node current_node = head;
    while(current_node != NULL)
    {
        if(current_node->key < 0) 
        {
            negative = insertAthead(negative, current_node->key);
        }
        current_node = current_node->next;
    }
    
    node final = NULL;
    current_node = head;
    while(current_node != NULL)
    {
        if(current_node->key < 0)
        {
            final = insertAtEnd(final, negative->key);
            negative = negative->next;
        }
        else
        {
            final = insertAtEnd(final, current_node->key);
        }
        current_node = current_node->next;
    }
    
    return final;
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
        // size_t pos = strcspn(input, "\\n");
        //input[pos] = '\\0';
        sscanf(input, "%s %d %d", word, &num1, &num2);
        if (strcmp(word, "f") == 0) {
            head = insertAthead(head, num1);
        } else if (strcmp(word, "t") == 0) {
            head = insertAtEnd(head, num1);
        } else if (strcmp(word, "a") == 0) {
            head = insertAfterAnother(head, num2, num1);
        } else if (strcmp(word, "b") == 0) {
            head = insertBeforeAnother(head, num2, num1);
        } else if (strcmp(word, "d") == 0) {
            head = deleteNode(head, num1);
        } else if (strcmp(word, "i") == 0) {
            head = deleteheadNode(head);
        } else if (strcmp(word, "l") == 0) {
            head = deleteLastnode(head);
        } else if (strcmp(word, "s") == 0) {
            searchNode(head, num1);
        } else if (strcmp(word, "r") == 0) {
            head = reverseList(head);
        } else if (strcmp(word, "ds") == 0) {
            printList(head);
        } else if (strcmp(word, "rn") == 0) {
            head = reverseNegative(head);
            printList(head);
        } else if (strcmp(word, "e") == 0) {
            break;
        }
    }
    return 1;
}