#include "../headers/conditions.h"
#include "../headers/chararray.h"
#include <stdio.h>
#include <stdlib.h>

void print_word(const char *arr)
{
	int i;

	if(arr == NULL)
		return;

	for(i = 0; !is_end_of_word(arr[i]); i++)
		putchar(arr[i]);
}

void print_reverse(const char **arr)
{
	int i, len;

	if(*arr == NULL)
		return;

	len = (int)(*arr)[0];

	for(i = len-1; i >= 0; i--) {
		if(is_space_char((*arr)[i]) || i == 0)
			print_word((*arr)+i+1);
		if(is_space_char((*arr)[i]))
			putchar((*arr)[i]);
	}

	putchar('\n');
}

int main()
{
	enum { max_buffer = 24 };
	char *char_array;
	char **p_array;
	int flag_eof = 0;

	p_array = &char_array;
	
	while(!flag_eof) {
		char_array = init_char_array(max_buffer);
		read_string_into_array(p_array, &flag_eof);
		print_reverse((const char**)p_array);
		free(char_array);
	}

	return 0;
}
