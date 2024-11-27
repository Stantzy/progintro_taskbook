#include <stdio.h>

const char *check_pattern(const char *str, const char *pat)
{
	const char *s_tmp, *p_tmp;

	for(; *str; str++) {
		for(s_tmp = str, p_tmp = pat; *s_tmp && *s_tmp == *p_tmp;
			s_tmp++, p_tmp++) {}
		if(!*p_tmp) {
			return str;
		}
	}

	return NULL;
}

void print_strings_with_pattern(char **strs, const char *pat)
{
	for(; *strs; strs++) {
		if(check_pattern(*strs, pat)) {
			printf("%s\n", *strs);
		}
	}
}

int count_occurences(const char *str, const char *pat)
{
	int counter = 0;
	
	while((str = check_pattern(str, pat))) {
		counter++;
		str++;
	}

	return counter;
}

void print_strings_with_pattern_and_count(char **strs, const char *pat)
{
	int counter = 0;

	for(; *strs; strs++) {
		if((counter = count_occurences(*strs, pat))) {
			printf("%s %d\n", *strs, counter);
		}
	}
}

int main(int argc, char **argv)
{
	enum { pattern_index = 1 };

	if(argc < 3) {
		fprintf(stderr,
		"Enter a pattern and one or more string as arguments!\n");
	}
	
	printf("a)\n");
	print_strings_with_pattern(argv+2, argv[pattern_index]);
	printf("\nb)\n");
	print_strings_with_pattern_and_count(argv+2, argv[pattern_index]);

	return 0;
}
