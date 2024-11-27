#include <stdio.h>
#include <stdlib.h>

typedef struct list_of_numbers {
	int data;
	struct list_of_numbers *prev, *next;
} list;

int is_condition_fulfilled(list *first, list *second)
{
	enum { task_condition = 5 };
	int x;

	x = first->data - second->data;
	if(x < 0)
		x = -x;

	return x <= task_condition;
}

void release_memory(list *first)
{
	list *tmp;
	
	if(!first)
		return;
	tmp = first;

	while(tmp) {
		first = tmp->next;
		free(tmp);
		tmp = first;
	}
}

list *read_numbers()
{
	list *tmp, *first = NULL, *last = NULL;
	int x;
	
	while(scanf("%d", &x) != EOF) {
		tmp = malloc(sizeof(list));
		if(!tmp) {
			release_memory(first);
			return NULL;
		}

		if(last) {
			tmp->data = x;
			tmp->prev = last;
			last->next = tmp;
			last = tmp;
		} else {
			first = last = tmp;
			first->data = x;
		}
		
		last->next = NULL;
		first->prev = NULL;
	}

	return first;
}

void print_pairs_of_numbers(list *first)
{
	list *tmp;

	if(!first)
		return;
	tmp = first;

	while(tmp->next) {
		if(is_condition_fulfilled(tmp, tmp->next)) {
			printf("%d %d", tmp->data, tmp->next->data);
			putchar('\n');
		}
		tmp = tmp->next;
	}
}

int main()
{
	list *head;

	head = read_numbers();
	if(head) {
		print_pairs_of_numbers(head);
		release_memory(head);
	}

	return 0;
}
