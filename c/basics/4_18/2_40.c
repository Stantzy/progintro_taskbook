#include <stdio.h>
#include <stdlib.h>

struct item {
	long x;
	int count;
	struct item *next;
};

/* finds a number in a list and if it found,
   returns a pointer to the item with the number else NULL */
struct item *find_number_in_list(struct item *first, long number)
{
	struct item *tmp;
	tmp = first;

	while(tmp != NULL) {
		if(tmp->x == number)
			return tmp;
		tmp = tmp->next;
	}

	return NULL;
}

/* finds a number that has occured more than three times
   and returns a pointer to this item else NULL */
struct item *find_ge_three(struct item *first)
{
	struct item *tmp;
	enum { task_condition = 3 };

	tmp = first;

	while(tmp != NULL) {
		if(tmp->count >= task_condition)
			return tmp;
		tmp = tmp->next;
	}

	return NULL;
}

void release_memory(struct item *first)
{
	struct item *tmp;
	while(tmp != NULL) {
		tmp = first->next;
		free(first);
		first = tmp;
	}
}

int main()
{
	struct item *first = NULL, *cur, *repeated;
	long readed_number;

	while(scanf("%ld", &readed_number) != EOF) {
		repeated = find_number_in_list(first, readed_number);
		if(repeated) {
			(repeated->count)++;
		}
		if(!first && !repeated) {
			first = malloc(sizeof(struct item));
			first->x = readed_number;
			first->count = 1;
			first->next = NULL;
			cur = first;
		}
		if(!repeated) {
			cur->next = malloc(sizeof(struct item));
			cur = cur->next;
			cur->x = readed_number;
			cur->count = 1;
			cur->next = NULL;
		}
	

		repeated = NULL;
	}

	cur = first;
	while(cur != NULL) {
		cur = find_ge_three(cur);
		if(cur != NULL) {
			printf("%ld\n", cur->x);
			cur = cur->next;
		}
	}
	
	if(first)
		release_memory(first);

	return 0;
}
