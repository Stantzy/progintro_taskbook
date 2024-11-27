#include <stdio.h>
#include <stdlib.h>

struct item {
	int x;
	struct item *next;
};

/* 4.17 a) */
struct item *int_arr_to_list_recursive(const int *numbers, int length)
{
	struct item *tmp;
	
	if(!length) {
		return NULL;
	}

	tmp = malloc(sizeof(struct item));
	tmp->x = *numbers;
	tmp->next = int_arr_to_list_recursive(numbers+1, length-1);

	return tmp;
}

struct item *int_arr_to_list_loop(const int *numbers, int length)
{
	int i;
	struct item *tmp, *first, *last;
	first = NULL;
	last = NULL;

	for(i = 0; i < length; i++) {
		tmp = malloc(sizeof(struct item));
		tmp->x = numbers[i];
		tmp->next = NULL;
	
		if(last) {
			last->next = tmp;
			last = last->next;
		} else {
			first = last = tmp;
		}
	}

	return first;
}

void release_memory_list(struct item *ptr)
{
	struct item *tmp;

	while(ptr != NULL) {
		tmp = ptr->next;
		free(ptr);
		ptr = tmp;
	}
}

void print_list(struct item *p_list)
{	
	while(p_list != NULL) {
		printf("%d ", p_list->x);
		p_list = p_list->next;
	}

	printf("\n");
}

/* 4.17 b) */
int count_items(struct item *first)
{
	int counter;
	struct item *tmp;
	tmp = first;
	
	do {
		counter++;
		tmp = tmp->next;
	} while(tmp != NULL);

	return counter;
}

int *list_to_arr(struct item *first)
{
	struct item *tmp;
	int item_counter, i;
	int *p_arr;

	tmp = first;
	item_counter = count_items(first);
	p_arr = malloc(sizeof(int)*item_counter);
	if(p_arr == NULL) {
		fputs("Memory allocation error.", stderr);
		return NULL;
	}
	
	p_arr[0] = item_counter;

	for(i = 1; tmp != NULL; i++) {
		p_arr[i] = tmp->x;
		tmp = tmp->next;
	}
	
	return p_arr;
}

void print_int_array(int* p_arr, int len)
{
	int i;
	if(!p_arr)
		return;

	for(i = 0; i < len; i++) 
		printf("%d ", p_arr[i]);

	printf("\n");
}

int main()
{
	int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	struct item *p_list_rec, *p_list_loop;
	int *new_arr;

	p_list_rec = int_arr_to_list_recursive(arr, sizeof(arr)/sizeof(int));
	print_list(p_list_rec);


	p_list_loop = int_arr_to_list_loop(arr, sizeof(arr)/sizeof(int));
	print_list(p_list_loop);


	new_arr = list_to_arr(p_list_loop);
	if(new_arr)
		print_int_array(new_arr, *new_arr+1);

	
	release_memory_list(p_list_rec);
	release_memory_list(p_list_loop);
	free(new_arr);
	
	return 0;
}
