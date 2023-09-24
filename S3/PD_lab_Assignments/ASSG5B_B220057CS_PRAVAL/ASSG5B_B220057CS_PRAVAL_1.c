#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct linkedList 
{
    int key;
    struct linkedList *next;
};

typedef struct linkedList *node;

node createNode()
{
    node new_node;
    new_node = (node) malloc(sizeof(struct linkedList));
    new_node->next = NULL;
    return new_node;
}

node insertAthead(node head, int key)
{
    node new_head, new_node;
    new_node = createNode();
    new_node->key = key;
    if(head == NULL) 
    {
        head = new_node;
    } 
    else 
    {
        new_node->next = head;
        head = new_node;
    }
    return head;
}

node insertAtEnd(node head, int key)
{
    node new_node, current_node;
    new_node = createNode();
    new_node->key = key;
    new_node->next = NULL;
    if (head == NULL) 
    {
        head = new_node;
    } 
    else 
    {
        current_node = head;
        while(current_node->next != NULL) 
        {
            current_node = current_node->next;
        }
        current_node->next = new_node;
    }
    return head;
}

node insertAfterAnother(node head, int exist_key, int key)
{
    node new_node, current_node, next_node;
    new_node = createNode();
    new_node->key = key;
    current_node = head;
    while(current_node->key != exist_key) 
    {
        current_node = current_node->next;
    }
    new_node->next = current_node->next;
    current_node->next = new_node;
    return head;
}

node insertBeforeAnother(node head, int exist_key, int key)
{
    node new_node, current_node, previous_node = NULL;
    new_node = createNode();
    new_node->key = key;
    current_node = head;
    while(current_node->key != exist_key) 
    {
        previous_node = current_node;
        current_node = current_node->next;
    }
    if(previous_node == NULL) 
    {
        return insertAthead(head, key);
    } 
    else 
    {
        previous_node->next = new_node;
        new_node->next = current_node;
    }
    return head;
}

node deleteheadNode(node head)
{
    if(head == NULL)
    {
        printf("-1\n");
        return head;
    }
    
    printf("%d\n", head->key);
    node new_head = head->next;
    free(head);
    return new_head;
}

node deleteNode(node head, int key)
{
    if(head == NULL) 
    {
      printf("-1\n");
      return head;
    }
    if(head->key == key) 
    {
        return deleteheadNode(head);
    }
    
    node current_node = head, previous_node, next_node;
    while(current_node != NULL && current_node->key != key) 
    {
        previous_node = current_node;
        current_node = current_node->next;
        if(current_node == NULL) 
        {
            printf("-1\n");
            return head;
        }
        next_node = current_node->next;
    }
    
    previous_node->next = next_node;
    printf("%d\n", current_node->key);
    free(current_node);
    return head;
}

node deleteLastnode(node head)
{
    if(head == NULL) 
    {
        printf("-1\n");
        return head;
    } 
    else if(head->next == NULL) 
    {
        return deleteheadNode(head);
    }
   else 
   {
        node current_node = head;
        node previous_node = NULL;
        while(current_node->next != NULL) 
        {
            previous_node = current_node;
            current_node = current_node->next;
        }
        printf("%d\n", current_node->key);
        previous_node->next = NULL;
        free(current_node);
        return head;
    }
}

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

void searchNode(node head, int key)
{
    if(head == NULL) 
    {
        printf("-1\n");
        return;
    }
    node current_node = head;
    while (current_node != NULL && current_node->key != key) 
    {
        current_node = current_node->next;
    }
    if(current_node == NULL) 
    {
        printf("-1\n");
    }
    else 
    {
        printf("1\n");
    }
}

node reverseList(node head)
{
    if(head == NULL || head->next == NULL) 
    {
        return head;
    }

    node current_node = head;
    node previous_node = NULL;
    node new_head = head;
    node temp_node;
    while(current_node != NULL) 
    {
        temp_node = current_node->next;
        new_head = current_node;
        new_head->next = previous_node;
        previous_node = current_node;
        current_node = temp_node;
    }
    return new_head;
}

node reverseEven(node head)
{  
   node current_node = head;
   node even = NULL;
   node odd = NULL;
   int n = 1;
   while(current_node != NULL)
   {
      if (n % 2 == 1)
      {
         odd = insertAtEnd(odd, current_node->key);
      }
      else
      {
         even = insertAthead(even, current_node->key);
      }
      current_node = current_node->next;
      n++;
   }
   node re_list = NULL;
   for(int i = 1; i < n; i++)
   {
      if (i % 2 == 1)
      {
         re_list = insertAtEnd(re_list, odd->key);
         odd = odd->next;
      }
      else
      {
         re_list = insertAtEnd(re_list, even->key);
         even = even->next;
      }
   }
   printList(re_list);
   head = re_list;
   return head;
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
            printList(head);
        } else if (strcmp(word, "ds") == 0) {
            printList(head);
        } else if (strcmp(word, "re") == 0) {
            head = reverseEven(head);
        } else if (strcmp(word, "e") == 0) {
            break;
        }
    }
   return 1;
}