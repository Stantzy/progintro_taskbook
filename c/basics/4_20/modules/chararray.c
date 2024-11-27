#include "../headers/chararray.h"
#include "../headers/conditions.h"
#include <stdio.h>
#include <stdlib.h>

char *init_char_array(int n)
{
	char *arr;
	int i;

	arr = malloc(sizeof(char) * n);
	if(!arr)
		return NULL;
	arr[0] = n;		/* will store the length */

	for(i = 1; i < n; i++) 
		arr[i] = 0;

 	return arr;
}

static void extend_char_array(char **arr)
{
	size_t new_arr_length;
	char *new_arr;
	int i;

	new_arr = malloc(sizeof(char) * (*arr)[0] + 1);
	new_arr_length = (*arr)[0] + 1;
	new_arr[0] = new_arr_length;
	
	for(i = 1; i < new_arr_length; i++) {
		if(i >= (*arr)[0])
			new_arr[i] = 0;
		else
			new_arr[i] = (*arr)[i];
	}
	
	free(*arr);

	*arr = new_arr;
}
	
void read_string_into_array(char **arr, int *flag_eof)
{
	char c;
	int counter = 1;
	
	while(1) {
		c = getchar();
		if(counter >= (*arr)[0])
			extend_char_array(arr);
		if(is_end_of_string(c)) {
			(*arr)[counter] = '\0';
			break;
		}
		(*arr)[counter] = c;
		counter++;
	}

	if(c == EOF)
		*flag_eof = 1;
}

/* 2_43 */
int find_max_word_length(const char *arr)
{
	int i, counter = 0, max_length = 0;

	for(i = 1; arr[i] != '\0'; i++) {
		if(is_end_of_word(arr[i])) {
			counter = 0;
			continue;
		}
		counter++;
		if(counter > max_length)
			max_length = counter;
	}
		
	return max_length;
}

/* 2_44 */
char *find_longest_digits_pos(const char *arr, int max)
{
	char *tmp, *sequence = NULL;
	int counter = 0;
	int i;

	if(arr == NULL)
		return NULL;
	
	for(i = 0; !is_end_of_string(arr[i]); i++) {
		if(is_digit(arr[i]))
			counter++;
		else
			counter = 0;
		
		if(counter == 1)
			tmp = (char *)&(arr[i]);

		if(counter == max) {
			sequence = tmp;
			return sequence;
		}
	}
	
	return sequence;
}

int find_longest_number_length(const char *arr)
{
	int length = 0, counter = 0;
	int i;

	for(i = 1; !is_end_of_string(arr[i]); i++) {
		if(is_digit(arr[i])) 
			counter++;
		else
			counter = 0;
		if(counter > length)
			length = counter;
	}

	return length;
}