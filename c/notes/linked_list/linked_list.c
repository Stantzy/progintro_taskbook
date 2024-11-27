#include <stdio.h>
#include <stdlib.h>

struct item {
	int data;
	struct item *next;
};

struct item *create_list(int n)
{
	struct item *first = NULL, *last = NULL, *tmp;
	int i;

	for(i = 0; i <= n; i++) {
		tmp = malloc(sizeof(struct item));
		if(last) {
			last->next = tmp;
			last = tmp;
		} else {
			first = last = tmp;
		}
	}

	last->next = NULL;

	return first;
}

void delete_list(struct item *ptr)
{
	while(ptr) {
		struct item *tmp = ptr;
		ptr = tmp->next;
		free(tmp);
	}
}

void fill_list(struct item *ptr)
{
	int n = 0;
	for(; ptr; ptr = ptr->next) {
		ptr->data = n;
		n++;
	}
}

void print_list_data(const struct item *ptr)
{
	for(; ptr; ptr = ptr->next) {
		printf("Data: %d\n", ptr->data);
	}
}

int list_sum(const struct item *ptr)
{
	return ptr ? ptr->data + list_sum(ptr->next) : 0;
};


int main()
{
	struct item *list;
	int sum;
	
	list = create_list(10);
	fill_list(list);
	sum = list_sum(list);

	print_list_data(list);
	printf("sum = %d\n", sum);

	delete_list(list);

	return 0;
}
