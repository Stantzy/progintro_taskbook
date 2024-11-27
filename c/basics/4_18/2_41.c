#include <stdio.h>
#include <stdlib.h>

#define MEMORY_ERROR "Memory allocation error."

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

int find_max_counter(struct item *first)
{
	int max = 0;
    struct item *tmp;
	tmp = first;

	while(tmp != NULL) {
		if(tmp->count > max) {
			max = tmp->count;
		}
		tmp = tmp->next;
	}

	return max;
}

void print_numbers(struct item *first)
{
    int max = 0;
    struct item *tmp;
    max = find_max_counter(first);
    tmp = first;
    while(tmp != NULL) {
        if(tmp->count == max)
            printf("%ld\n", tmp->x);
        tmp = tmp->next;
    }
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
	int max = 0;

	while(scanf("%ld", &readed_number) != EOF) {
		repeated = find_number_in_list(first, readed_number);
		if(repeated) {
			(repeated->count)++;
		}
		if(first && !repeated) {
			cur->next = malloc(sizeof(struct item));
			if(cur->next == NULL) {
				fputs(MEMORY_ERROR, stderr);
				return 1;
			}
			cur = cur->next;
			cur->x = readed_number;
			cur->count = 1;
			cur->next = NULL;
		}		
        if(!first && !repeated) {
			first = malloc(sizeof(struct item));
			if(first == NULL) {
				fputs(MEMORY_ERROR, stderr);
				return 1;
			}
			first->x = readed_number;
			first->count = 1;
			first->next = NULL;
			cur = first;
		}

	
		repeated = NULL;
	}

	if(first) {
    	print_numbers(first);
		release_memory(first);
	}
	
	return 0;
}
