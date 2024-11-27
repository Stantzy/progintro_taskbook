#include <stdio.h>
#include <stdlib.h>

struct string {
	char c;
	int pos;
	struct string *next;
};

int is_end_of_string(char c)
{
	return c == '\n' || c == EOF || c == '\0';
}

int is_end_of_word(struct string *word)
{
	return word->c == '\0' || word->c == ' ' || word->c == '\t';
}

int is_space_char(char c)
{
	return c == ' ' || c == '\t';
}

int is_to_print_space(int count, int i, int max_pos, struct string *c)
{
	return !c->pos && count-1 <= max_pos && i > count-1;
}

struct string *read_string(int *flag_eof)
{
	struct string *first, *tmp;
	char chr;
	int position = 0;

	first = malloc(sizeof(struct string));
	if(!first)
		return NULL;
	tmp = first;

	while(1) {
		chr = getchar();
		if(is_end_of_string(chr)) {
			tmp->c = '\0';
			tmp->pos = 0;
			tmp->next = NULL;
			break;
		}
		position++;
		if(is_space_char(chr))
			position = 0;
		tmp->c = chr;
		tmp->pos = position;
		tmp->next = malloc(sizeof(struct string));
		tmp = tmp->next;
	}

	if(chr == EOF)
		*flag_eof = 1;

	return first;
}

int find_max_position(struct string *s)
{
	int max_pos = 0;
	struct string *tmp;
	tmp = s;
	
	while(tmp->c != '\0') {
		if(tmp->pos > max_pos)
			max_pos = tmp->pos;
		tmp = tmp->next;
	}
	
	return max_pos;
}

void print_string_in_column(struct string *s)
{
	struct string *tmp;
	int i, max_pos, cur_pos;

	tmp = s;
	if(!tmp)
		return;

	max_pos = find_max_position(s);

	for(i = 1; i <= max_pos; i++) {
		int count;
		count = 0;
		while(tmp) {
			count++;
			if(tmp->pos == i)
				putchar(tmp->c);
			if(is_to_print_space(count, i, max_pos, tmp)) {
				putchar(' ');
				count = 0;
			}
			tmp = tmp->next;
		}

		putchar('\n');
		tmp = s;
	}
}


void release_memory(struct string *s)
{
	struct string *tmp;

	if(!s)
		return;
	tmp = s;

	while(tmp) {
		s = tmp->next;
		free(tmp);
		tmp = s;
	}
}

int main()
{
	int flag_eof = 0;
	struct string *s;

	while(!flag_eof) {
		s = read_string(&flag_eof);
		if(flag_eof)
			putchar('\n');
		print_string_in_column(s);
		release_memory(s);
	}

	return 0;
}
