#include "../headers/conditions.h"
#include "../headers/chararray.h"
#include <stdio.h>
#include <stdlib.h>

#define MEM_ALLOC_ERR "line 18: Memory allocation error in init_char_array."

void print_longest_digits_sequences(const char *arr)
{
	char *digits_to_print;
	int max_length;
	
	max_length = find_longest_number_length(arr);
	digits_to_print = find_longest_digits_pos(arr, max_length);
	
	if(digits_to_print == NULL || max_length == 0)
		return;

	while(digits_to_print) {	
		if(is_digit(*digits_to_print)) {
			putchar(*digits_to_print);
			digits_to_print++;
		} else {
			digits_to_print = 
				find_longest_digits_pos(
						digits_to_print,
						max_length
						);
			if(digits_to_print == NULL)
				putchar('\n');
			else
				putchar(' ');
		}
	}
}


int main()
{
	enum { start_buffer = 24 };
	char *char_arr;
	char **p_arr;
	int flag_eof = 0;

	p_arr = &char_arr;

	while(!flag_eof) {
		char_arr = init_char_array(start_buffer);
		if(char_arr == NULL) {
			fputs(MEM_ALLOC_ERR, stderr);
			return 1;
		}
		read_string_into_array(p_arr, &flag_eof);
		print_longest_digits_sequences(char_arr);
		free(char_arr);
	}

	return 0;
}
