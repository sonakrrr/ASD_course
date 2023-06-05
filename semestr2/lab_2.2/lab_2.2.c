#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct linked_list
{
	int data;
	struct linked_list * next_p, * prev_p;
} l_list;

void list_push(l_list * head_p)
{

	l_list * item = (l_list *)malloc(sizeof(l_list));

	printf("Enter value to add:\n");
	scanf("%d", &(item -> data));

	item -> next_p = NULL;
	item -> prev_p = NULL;

	l_list * curr = head_p;

	while (curr -> next_p != NULL)
	{
		curr = curr -> next_p;
	}

	curr -> next_p = item;
	item -> prev_p = curr;
}

void list_pop(l_list ** head_pp)
{

	l_list * curr = (* head_pp);
	while (curr -> next_p != NULL)
	{
		curr = curr -> next_p;
	}

	if (curr -> prev_p == NULL)
	{
		free(curr);
		(* head_pp) = NULL;
	}
	else
	{
		l_list * prev = curr -> prev_p;
		prev -> next_p = NULL;
		free(curr);
	}
}

void list_unshift(l_list ** head_pp)
{

	l_list * item = (l_list *)malloc(sizeof(l_list));

	printf("Enter value to add:\n");
	scanf("%d", &(item->data));

	item -> next_p = * head_pp;
	item -> prev_p = NULL;

	(* head_pp) -> prev_p = item;
	(* head_pp) = item;
}

void list_shift(l_list ** head_pp)
{

	l_list * curr = (* head_pp);

	if (curr -> next_p == NULL)
	{
		free(curr);
		(* head_pp) = NULL;
	}
	else
	{
		l_list * next = curr -> next_p;
		next -> prev_p = NULL;
		(* head_pp) = next;
		free(curr);
	}
}

void list_printing(l_list * head_p)
{

	l_list * curr = head_p;

	printf("List: ");
	while (curr -> next_p != NULL)
	{
		printf("%d --> ", curr -> data);
		curr = curr -> next_p;
	}
	printf("%d\n", curr -> data);
}

l_list * list_creating(int len)
{
	if (len < 1)
	{
		printf("Error!\n");
		return NULL;
	}

	l_list * head_p = (l_list *)malloc(sizeof(l_list));

	int count = 1;
	printf("Enter value to add at the #%d position: ", count);
	scanf("%d", &(head_p -> data));

	head_p -> next_p = NULL;
	head_p -> prev_p = NULL;

	l_list * curr = head_p;
	while (count < len)
	{
		l_list * item = (l_list *)malloc(sizeof(l_list));

		printf("Enter value to add at the #%d position: ", ++count);
		scanf("%d", &(item -> data));
		item -> prev_p = curr;
		item -> next_p = NULL;

		curr -> next_p = item;
		curr = curr -> next_p;
	}

	list_printing(head_p);
	return head_p;
}

int calculate_expression(l_list * head_p) {

    int expression_result = 1;
    l_list * curr = head_p;
    l_list * last = NULL;

    while (curr -> next_p != NULL) {
        curr = curr -> next_p;
    }
    last = curr;

    curr = head_p;

    while (curr != NULL && curr -> next_p != NULL && curr != last) {
        expression_result *= (curr -> data + curr -> next_p -> data + 2 * last -> data);
        curr = curr -> next_p;
        last = last -> prev_p;
    }

    return expression_result;
}

void list_deleting(l_list ** head_pp)
{

	l_list * curr = * head_pp;
	while (curr != NULL)
	{
		l_list * prev = curr;
		curr = curr -> next_p;
		free(prev);
		printf("Memory is freed.\n");
	}
	(* head_pp) = NULL;
}

int main()
{
	int len;
	printf("Enter the length: ");
	scanf("%d", &len);

	l_list * list_p = list_creating(len);

	printf("The result of the expression is: %d\n", calculate_expression(list_p));

	list_deleting(&list_p);
	return 0;
}
