#include <stdio.h>

int is_valid_string(const char *str)
{
	while(str) {
		if(*str == 0)
			return 0;
		str++;
	}
	return 1;
}

void remove_spaces_from_string(char *str)
{	
	char *src, *dest;
	src = str;
	dest = str;

	while(src) {
		if(*src == ' ') {
			src++;
			continue;
		} else {
			*dest = *src;
		}

		if(*dest == 0)
			break;

		src++;
		dest++;
	}
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		fputs("Input string as a command line argument.\n", stderr);
	}

	if(is_valid_string(argv[1])) {
		return 1;
	}

	printf("%s\n", argv[1]);
	remove_spaces_from_string(argv[1]);
	printf("%s\n", argv[1]);

	return 0;
}
