#include<stdio.h>
#include<stdlib.h>

struct ListNode
{
	int val;
	struct ListNode* next;
};

typedef struct ListNode *node;

node createNode(node head, int val)
{
	node new_node = (node)malloc(sizeof(struct ListNode));
	new_node->val = val;
	new_node->next = NULL;
	return new_node;
}

node insertNode(node head, int val)
{	
	node new_node = createNode(head, val);
	if(head == NULL)
	{
		return new_node;
	}
	node traverse_node = head;
	while(traverse_node->next != NULL)
	{
		traverse_node = traverse_node->next;
	}
	traverse_node->next = new_node;
	return head;
}

node cloneList(node head, node clone)
{
	node traverse_node = head;
	while(traverse_node != NULL)
	{
		clone = insertNode(clone, traverse_node->val);
		traverse_node = traverse_node->next;
	}
	return clone;
}

node deleteNode(node head, int n)
{
	if(head == NULL)
	{
		printf("-1\n");
		return head;
	}
	node traverse_node = head;
	node previous_node = NULL;
	node next_node = NULL;
	if(n == 1)
	{
		printf("%d\n", head->val);
		previous_node = head->next;
		free(head);
		return previous_node;
		
	}
	for(int i = 1; i < n; i++)
	{
		previous_node = traverse_node;
		traverse_node = traverse_node->next;
		if(traverse_node == NULL)
		{
			printf("-1\n");
			return head;
		}
	}
	printf("%d\n", traverse_node->val);
	next_node = traverse_node->next;
	previous_node->next = next_node;
	free(traverse_node);
	return head;
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

void displayList(node head)
{
	if(head == NULL)
	{
		printf("-1\n");
		return;
	}
	node traverse_node = head;
	while(traverse_node != NULL)
	{
		printf("%d ", traverse_node->val);
		traverse_node = traverse_node->next;
	}
	printf("\n");
}

node palindrome(node head)
{
	if(head == NULL)
	{
		printf("-1\n");
		return head;
	}
	node clone = NULL;
	clone = cloneList(head, clone);
	node reverse_list = reverseList(clone);
	node traverse_node_1 = head;
	node traverse_node_2 = reverse_list;
	while(traverse_node_1 != NULL && traverse_node_2 != NULL)
	{
		if(traverse_node_1->val != traverse_node_2->val)
		{
			printf("F\n");
			return head;
		}
		traverse_node_1 = traverse_node_1->next;
		traverse_node_2 = traverse_node_2->next;
	}
	printf("T\n");
	return head;
}

node removeDuplicates(node head)
{
	int i = 0, j = 0;
	node traverse_node = head;
	node prev_node = NULL;
	node temp;
	while (traverse_node != NULL)
	{
		node another_traverse_node = traverse_node->next;
		prev_node = traverse_node;
		while(another_traverse_node != NULL)
		{
			if(traverse_node->val == another_traverse_node->val)
			{
				prev_node->next = another_traverse_node->next;
				temp = another_traverse_node;
				another_traverse_node = another_traverse_node->next;
				free(temp);
			}
			else
			{
				prev_node = another_traverse_node;
				another_traverse_node = another_traverse_node->next;
			}
		}
		traverse_node = traverse_node->next;
	}
	displayList(head);
	return head;
}

int main()
{
	node head = NULL;
	char ch;
	int val;
	while(1)
	{
		scanf("%c", &ch);
		if(ch == 'a')
		{
			scanf(" %d", &val);
			head = insertNode(head, val);
		}
		else if(ch == 'r')
		{
			scanf(" %d", &val);
			head = deleteNode(head, val);
		}
		else if(ch == 'd')
		{
			head = removeDuplicates(head);
		}
		else if(ch == 'p')
		{
			head = palindrome(head);
		}
		else if(ch == 's')
		{
			displayList(head);
		}
		else if(ch == 'e')
		{
			break;
		}
	}
	return 1;
}
