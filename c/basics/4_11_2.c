#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum { tab = 9, space = 32 };

int check_argc(int argc)
{
	enum { max_argc = 2 };
	if(argc > max_argc || argc == 1) {
		return 1;
	}
	return 0;
}

int is_to_count_words(int c, int t)
{
	return (t != space && t != tab && t != 0) &&
		((c == '\n') || (c == space || c == tab || c == 0));
}

int count_words(const char *str)
{
	char prev, cur;
	int words = 0;

	if(*str == 0) {
		return 0;
	}

	prev = cur = *str;
	while(*str) {
		if(is_to_count_words(cur, prev)) {
			words++;
		}
		prev = cur;
		str++;
		cur = *str;
	}

	if(prev != space) {
		words++;
	}
	
	return words;	
}

int main(int argc, char **argv)
{
	int words = 0;

	if(check_argc(argc)) {
		printf("ERROR: only one command line argument is allowed\n");
		return 1;
	}

	words = count_words(argv[1]);
	printf("Words: %d\n", words);

	return 0;
}
