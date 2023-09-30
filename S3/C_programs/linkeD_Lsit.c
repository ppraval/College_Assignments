#include<stdio.h>
#include<stdlib.h>

struct LinkedList{
 int data;
 struct LinkedList *next;
};

typedef struct LinkedList *node;

node createNode() {
 node temp;
 temp = (node ) malloc (sizeof(struct LinkedList));
 
 if(temp == NULL)
    { 
        printf("Error creating a new node.\n");
        exit(0); 
    }
 temp->next = NULL;
 return temp; 
}


node insert_at_begin(int data, node first)
{
    node new_head;
    node new_node =createNode();
    new_node->data = data;
    new_node->next = first;
    new_head = new_node;
    return new_head;
}
node insert_at_end(int data, node first)
{
    node temp,p; 
    temp = createNode();
    temp->data = data;
    temp->next=NULL; 
    if(first == NULL) { first = temp; } 
    else 
    {
      p = first; 
      while(p->next != NULL) 
        p = p->next; 
      p->next = temp; 
    }
    return first;
}

void printlist(node head)
{
  if(head==NULL)
    printf("List is empty\n");
  else
  {
    while(head!=NULL)
    {
      printf("%d-->",head->data);
      head=head->next;
    }
  }
  printf("\b\b\b");
}

int main()
{
  node head1 = NULL;
  node head2 = NULL;
  node temp = NULL;

//   head = insert_at_end(9, head);
//   head = insert_at_begin(10, head);
//   head = insert_at_end(8, head);
//   head = insert_at_begin(20, head);
//   head = insert_at_end(5, head);
    int n, m, val;
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
    {
        scanf("%d", &val);
        head1 = insert_at_begin(val, head1);
    }
    scanf("%d", &m);
    for(int i = 0; i < m; i++)
    {
        scanf("%d", &val);
        head2 = insert_at_begin(val, head2);
    }
    if(head1 != NULL)
    {
        for (temp = head1; temp->next != NULL; temp = temp->next);
        temp->next = head2;
    }
    else
    {
        head1 = head2;
    }
    printlist(head1);
}