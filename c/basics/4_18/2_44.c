#include <stdio.h>
#include <stdlib.h>

typedef struct char_sequence {
	char x;
	int degree;	/* to count sequence of numbers */ 
	struct char_sequence *next;
} chrseq;

int is_end_of_string(char c)
{
	return c == '\n' || c == EOF || c == '\0';
}

int is_number(char c)
{
	return c >= '0' && c <= '9';
}

void release_memory(chrseq *first)
{
	chrseq *tmp;

	tmp = first;
	while(tmp) {
		first = tmp->next;
		free(tmp);
		tmp = first;
	}
}

chrseq *read_string(int *flag_eof)
{
	chrseq *first, *tmp;
	char c;
	int counter_of_numbers = 0;

	first = malloc(sizeof(chrseq));
	if(!first)
		return NULL;
	tmp = first;
	first->next = NULL;
	first->degree = 0;

	while(1) {
		c = getchar();
		if(is_end_of_string(c)) {
			tmp->x = '\0';
			tmp->degree = 0;
			tmp->next = NULL;
			break;
		}
		tmp->x = c;
		if(is_number(c)) {
			counter_of_numbers++;
			tmp->degree = counter_of_numbers;
		} else {
			counter_of_numbers = 0;
			tmp->degree = counter_of_numbers;
		}
		tmp->next = malloc(sizeof(chrseq));
		tmp = tmp->next;
		if(!tmp) {
			release_memory(first);
			return NULL;
		}
	}

	if(c == EOF)
		*flag_eof = 1;
	
	return first;
}

int return_max_degree(chrseq *first)
{
	chrseq *tmp;
	int max = 0;

	if(!first)
		return 0;
	tmp = first;

	while(tmp) {
		if(tmp->degree > max)
			max = tmp->degree;
		tmp = tmp->next;
	}

	return max;
}


chrseq *find_max_degree(chrseq *first)
{
	chrseq *tmp, *p_seq = NULL;
	int max;

	if(!first)
		return NULL;
	tmp = first;
	max = return_max_degree(first);

	while(tmp) {
		if(tmp->degree == 1)
			p_seq = tmp;
		if((tmp->degree == max) && p_seq)
			return p_seq;
		tmp = tmp->next;
	}

	return NULL;
}

void output_numbers(chrseq *numbers_head)
{
	chrseq *tmp;
	
	if(!numbers_head)
		return;
	tmp = numbers_head;

	while(tmp) {
		if(is_number(tmp->x))
			putchar(tmp->x);
		if(is_number(tmp->x) && (tmp->next->degree == 0))
				break;
		tmp = tmp->next;
	}
}

void print_longest_sequences_of_numbers(chrseq *first)
{
	chrseq *tmp;
	
	if(!first || first->x == '\0')
		return;
	tmp = find_max_degree(first);

	while(tmp) {
		output_numbers(tmp);
		tmp = find_max_degree(tmp->next);
		if(tmp)
			putchar(' ');
	}

	putchar('\n');
}

int main()
{
	chrseq *seq;
	int flag_eof = 0;

	while(!flag_eof) {
		seq = read_string(&flag_eof);
		if(flag_eof) {
			putchar('\n');
			break;
		}
		print_longest_sequences_of_numbers(seq);
		release_memory(seq);
	}

	return 0;
}
