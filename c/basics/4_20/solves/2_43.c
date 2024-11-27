#include "../headers/conditions.h"
#include "../headers/chararray.h"
#include <stdio.h>
#include <stdlib.h>

#define MEM_ALLOC_ERR "Memory allocation error."

int is_to_print_space(char c, int p1, int p2, int space_flag)
{
	return is_end_of_word(c) && (p1 <= p2) && !space_flag;
}

void print_vertically(const char *arr)
{
	int i, j;
	int max_len, space_flag = 0;

	max_len = find_max_word_length(arr);

	for(i = 1; i <= max_len; i++) {
        int pos_current = 1;
		int pos_to_print = i;
		for(j = 1; arr[j] != '\0'; j++) {
            if(is_to_print_space(arr[j],
						pos_current,
						pos_to_print,
                        space_flag)) {
				putchar(' ');
			}
			
			if(is_end_of_word(arr[j])) {
				pos_current = 1;
                space_flag = 1;
				continue;
			} else {
                space_flag = 0;
            }

			if(pos_current == pos_to_print) 
				putchar(arr[j]);

            pos_current++;
		}

        putchar('\n');
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
		print_vertically(char_arr);
		free(char_arr);
	}
	
	return 0;
}
