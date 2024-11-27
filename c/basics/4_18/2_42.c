#include <stdio.h>
#include <stdlib.h>

struct string {
	char chr;
	struct string *next, *prev;
};

int is_end_of_string(char c)
{
	return c == EOF || c == '\n' || c == '\0';

}

int is_end_of_word(struct string *word)
{
	return !(word->chr != '\0' && word->chr != ' ' && word->chr != '\t');
}

void release_memory(struct string *first_char);

struct string *read_string(int *flag_eof)
{
	struct string *first, *cur, *tmp;
	char c;

	first = malloc(sizeof(struct string));
	if(!first)
		return NULL;
	
	first->prev = malloc(sizeof(struct string));
	if(!first->prev)
		return NULL;
	first->prev->chr = '\0';
	first->prev->prev = NULL;
	first->prev->next = first;
	
	cur = first;
	
	while(1) {
		c = getchar();
		if(is_end_of_string(c))
			break;
		tmp = cur;
		cur->chr = c;
		cur->next = malloc(sizeof(struct string));
		cur = cur->next;
		if(!cur)
			release_memory(first);
			return NULL;
		cur->prev = tmp;
	}
	cur->chr = '\0';
	cur->next = NULL;
	
	if(c == EOF)
		*flag_eof = 1;

	return first;
}

struct string *get_last_char(struct string *first)
{
	struct string *tmp;
	tmp = first;
	if(!tmp)
		return NULL;
	while(tmp->next) {
		tmp = tmp->next;
	}
	if(tmp->prev->chr == '\n')
		return tmp->prev->prev;
	return tmp->prev;
}

void print_word(struct string *word)
{
	struct string *spc;
	spc = word->prev;
	while(!is_end_of_word(word)) {
		putchar(word->chr);
		word = word->next;
	}
	putchar(spc->chr);
}

void reverse_print_string(struct string *str)
{
	struct string *last_char, *tmp;

	last_char = get_last_char(str);
	while(last_char) {
		tmp = last_char;
		while(!is_end_of_word(tmp)) {
			tmp = tmp->prev;
		}
		if(tmp)
			print_word(tmp->next);
		last_char = tmp->prev;
	}
}

void release_memory(struct string *first_char)
{
	struct string *tmp;
	tmp = first_char;
	if(!tmp)
		return;
	while(tmp) {
		first_char = tmp->next;
		free(tmp);
		tmp = first_char;
	}
}

int main()
{
	struct string *readed_string;
	int flag_eof = 0;
	while(!flag_eof) {
		readed_string = read_string(&flag_eof);
		if(flag_eof)
			putchar('\n');
		reverse_print_string(readed_string);
		release_memory(readed_string);
		putchar('\n');
	}	
}
