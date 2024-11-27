#include <stdio.h>

/* a) */
int count_symbols(const char *str)
{
	int i = 0;
	while(str[i]) {
		i++;
	}
	return i;
}

void print_longest(int argc, char **argv)
{
	int i, len, max_len, max_index;

	len = 0;
	max_len = 0;

	printf("a) ");
	for(i = 1; i < argc; i++) {
		len = count_symbols(argv[i]);
		if(len > max_len) {
			max_len = len;
			max_index = i;
		}
	}

	printf("%s\n", argv[max_index]);
}

/* b) */
int find_duplicates(const char *str)
{
	int i, j, flag;
	i = 0;
	j = 1;
	flag = 1;

	while(str[i]) {
		while(str[i+j]) {
			if(str[i] == str[i+j]) {
				flag = 0;
			}
			j++;
		}
		i++;
		j = 1;
	}

	return flag;
}


void print_no_duplicates(int argc, char **argv)
{
	int i, flag;
	flag = 0;

	printf("b) ");
	for(i = 1; i < argc; i++) {
		flag = find_duplicates(argv[i]);
		if(flag) {
			printf("%s ", argv[i]);
		}
	}
	printf("\n");
}

/* c) */
int check_dots(const char *str)
{
	while(*str) {
		if(*str == '.') {
			return 1;
		}
		str++;
	}
	return 0;
}

int check_one_atsign(const char *str)
{
	int flag = 0;
	
	while(*str) {		
		if(*str == '@') {
			flag++;
		}
		if(flag > 1) {
			return 0;
		}
		str++;
	}

	return flag;
}

void print_c(int argc, char **argv)
{
	int i, flag_dot = 0, flag_atsign = 0;

	printf("c) ");
	for(i = 1; i < argc; i++) {
		flag_dot = check_dots(argv[i]);
		flag_atsign = check_one_atsign(argv[i]);

		if(flag_dot && flag_atsign) {
			printf("%s ", argv[i]);
		}

		flag_dot = flag_atsign = 0;
	}

	printf("\n");
}

/* d) */
int is_digit(char c)
{
	if(c < '0' || c > '9') {
		return 0;
	}
	return 1;
}

int check_only_digits(const char *str)
{
	while(*str) {
		if(!is_digit(*str)) {
			return 0;
		}
		str++;
	}
	return 1;
}

void print_only_digits(int argc, char **argv)
{
	int i, flag = 0;
	
	printf("d) ");
	for(i = 1; i < argc; i++) {
		flag = check_only_digits(argv[i]);
		if(flag) {
			printf("%s ", argv[i]);
		}
		flag = 0;
	}

	printf("\n");
}

/* e) */
int check_only_one_symbol(const char *str)
{
	char c = *str;
	while(*str) {
		if(*str != c) {
			return 0;
		}
		str++;
	}
	return 1;
}

void print_only_one_symbol(int argc, char **argv)
{
	int i, flag = 0;
	
	printf("e) ");
	for(i = 1; i < argc; i++) {
		flag = check_only_one_symbol(argv[i]);
		if(flag) {
			printf("%s ", argv[i]);
		}
		flag = 0;
	}

	printf("\n");
}

/* f) */
int is_latin(char c)
{
	if((c > 'a' && c < 'z') || (c > 'A' && c < 'Z')) {
		return 1;
	}
	return 0;
}

int check_latin(const char *str)
{
	while(*str) {
		if(is_latin(*str)) {
			return 1;
		}
		str++;
	}
	return 0;
}
		
void print_at_least_one_latin(int argc, char **argv)
{
	int i, flag = 0;
	
	printf("f) ");
	for(i = 1; i < argc; i++) {
		flag = check_latin(argv[i]);
		if(flag) {
			printf("%s ", argv[i]);
		}
		flag = 0;
	}

	printf("\n");
}

/* g) */
int check_common_char(const char *sample, const char *compared)
{
	int i, j;
	
	for(i = 0; sample[i]; i++) {
		for(j = 0; compared[j]; j++) {
			if(sample[i] == compared[j]) {
				return 1;
			}
		}
	}

	return 0;
}

void print_common_char_with_first(int argc, char **argv)
{
	int i, flag = 0;
	
	printf("g) ");
	if(argc == 2) {
		return;
	}

	for(i = 2; i < argc; i++) {
		flag = check_common_char(argv[1], argv[i]);
		if(flag) {
			printf("%s ", argv[i]);
		}
		flag = 0;
	}

	printf("\n");
}

int main(int argc, char **argv)
{
	/* No command-line args */
	if(argc < 2) {
		return 0;
	}
	
	print_longest(argc, argv);		/* a) */
	print_no_duplicates(argc, argv);	/* b) */
	print_c(argc, argv);			/* c) */
	print_only_digits(argc, argv);		/* d) */
	print_only_one_symbol(argc, argv);	/* e) */
	print_at_least_one_latin(argc, argv);	/* f) */
	print_common_char_with_first(argc, argv); /* g */

	return 0;
}
