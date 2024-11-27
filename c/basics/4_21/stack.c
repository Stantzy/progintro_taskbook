#include "stack.h"
#include <stdlib.h>

struct stackdbl {
	int size_stack;
	int size_array;
	double *start;
	double *end;
	double *current;
};

struct stackdbl *stackdbl_init(int start_buffer)
{
	struct stackdbl *p_stack;
	int i;

	p_stack = malloc(sizeof(struct stackdbl));
	if(p_stack == NULL)
		return NULL;

	p_stack->start = malloc(sizeof(double) * start_buffer);
	if(p_stack->start == NULL)
		return NULL;

	for(i = 0; i < start_buffer; i++) {
		(p_stack->start)[i] = 0;
	}

	p_stack->size_stack = 0;
	p_stack->size_array = start_buffer;
	p_stack->current = p_stack->start;
	p_stack->end = p_stack->start + (sizeof(double) * start_buffer);

	return p_stack;
}

void stackdbl_destroy(struct stackdbl *p_stack)
{
	if(p_stack->start) {
		free(p_stack->start);
		p_stack->size_stack = 0;
		p_stack->size_array = 0;
		p_stack->start = NULL;
		p_stack->end = NULL;
		p_stack->current = NULL;
		free(p_stack);
	}
}

int stackdbl_empty(struct stackdbl *p_stack)
{
	return p_stack->size_stack == 0;
}

static void extend_stack(struct stackdbl *p_stack)
{
	int new_size, i;
	double *extended_array;

	new_size = p_stack->size_array * 2;
	extended_array = malloc(sizeof(double) * new_size);

	for(i = 0; i < new_size; i++) {
		if(i < p_stack->size_array)
			extended_array[i] = (p_stack->start)[i];
		else
			extended_array[i] = 0;
	}

	free(p_stack->start);

	p_stack->start = extended_array;
	p_stack->current = p_stack->start + 
			(sizeof(double) * p_stack->size_array);
	p_stack->size_array = new_size;
	p_stack->end = p_stack->start + sizeof(double) * new_size;
}

int stackdbl_push(struct stackdbl *p_stack, double x)
{
	if(p_stack->start == NULL)
		return 1;

	p_stack->current = p_stack->current + sizeof(double);
	(p_stack->size_stack)++;

	if(p_stack->size_stack > p_stack->size_array)
		extend_stack(p_stack);

	*(p_stack->current) = x;

	return 0;
}

double stackdbl_pop(struct stackdbl *p_stack)
{
	double data;
	if(p_stack->size_stack == 0)
		return 0;

	data = *(p_stack->current);
	*(p_stack->current) = 0;
	p_stack->current = p_stack->current - sizeof(double);
	(p_stack->size_stack)--;

	return data;
}