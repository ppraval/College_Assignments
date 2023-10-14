    #include<stdio.h>
    #include<stdlib.h>

    struct node_list 
    {
        int p;
        int jid;
        struct node_list *next;
    };

    typedef struct node_list *node;

    node createNode(int p, int jid)
    {
        node new_node;
        new_node = (node) malloc(sizeof(struct node_list));
        new_node->p = p;
        new_node->jid = jid;
        new_node->next = NULL;
        return new_node;
    }

    node add(node head, int jid, int p)
    {
        node new_node, current_node, previous_node = NULL;
        new_node = createNode(p, jid);

        if(head == NULL)
        {
            head = new_node;
            return head;
        }
        
        current_node = head;
        while(current_node != NULL && current_node->p >= p) 
        {
            if(current_node->p == p && current_node->jid > jid) 
            {
                break;
            }
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

    node schedule(node head)
    {
        if(head == NULL)
        {
            printf("-1\n");
            return head;
        }
        
        printf("%d\n", head->jid);
        node new_head = head->next;
        free(head);
        return new_head;
    }

    void display(node head)
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
                printf("%d %d\n", current_node->jid, current_node->p);
                current_node = current_node->next;
            }
        }
    }

    void next_job(node head)
    {
        if(head == NULL)
        {
            printf("-1\n");
        }
        else
        {
            printf("%d\n", head->jid);
        }
    }

    node replace_priority(node head, int jid, int np)
    {
        
        node current_node, previous_node = NULL;
        if (head == NULL)
        {
            printf("%d\n", -1);
        }
        else
        {
            current_node = head;
            while(current_node != NULL && current_node->jid != jid)
            {
                previous_node = current_node;
                current_node = current_node->next;
            }
            
            if (previous_node == NULL)
            {
                head = current_node->next;
            }
            else
            {
                previous_node->next = current_node->next;
            }
    
            head = add(head, jid, np);
        }
        return head;
    }

    int main()
    {
        int MAX = 1000000;
        char input[MAX];
        char command;
        int num1, num2;

        node head = NULL;

        while(1) {
            fgets(input, MAX - 1, stdin);
            sscanf(input, "%c %d %d", &command, &num1, &num2);
            if (command == 'a') 
            {
                head = add(head, num1, num2);

            }
            else if (command == 's') 
            {
                head = schedule(head);
            }
            else if (command == 'n') 
            {
                next_job(head);
            } 
            else if (command == 'r') 
            {
                head = replace_priority(head, num1, num2);
                
            }
            else if (command == 'd') 
            {
                display(head);
            }
            else if (command == 'e') 
            {
                break;
            }
        }
       return 1;
    }

