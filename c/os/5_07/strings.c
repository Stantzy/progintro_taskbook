#include "strings.h"

int compare_strings(char *item_1, char *item_2)
{
	int i;
	
	for(i = 0; item_1[i] != '\0' || item_2[i] != '\0'; i++) {
		if(item_1[i] != item_2[i])
			return 0;
	}

	return 1;
}

int count_len(char *item)
{
	int i;
	for(i = 0; item[i] != '\0'; i++);
	return i;
}

static void reverse_arr(char *start, char *end)
{
	while(start < end) {
		char tmp = *start;
		*start++ = *end;
		*end-- = tmp;
	}	
}

void int_to_str(char *str, int number)
{
    enum { added_ascii_code = 48 };
	int i = 0;

	if(number == 0) {
		str[i] = 0;
		i++;
	}

	while(number != 0) {
		str[i] = (number % 10) + added_ascii_code;
		number /= 10;
		i++;
	}

	str[i] = '\0';
	reverse_arr(str, str+i-1);
}