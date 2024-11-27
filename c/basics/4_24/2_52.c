#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char *longest, *longest_cur;
	FILE *f_cur;
	int i, c, count = 0, max = 0;

	if(argc < 2) {
		fprintf(stderr, "Provide the name of at least one file\n");
		return 1;
	}

	longest = malloc(sizeof(char) * BUFSIZ);
	longest_cur = malloc(sizeof(char) * BUFSIZ);

	for(i = 1; i < argc; i++) {
		f_cur = fopen(argv[i], "r");
		while((c = fgetc(f_cur)) != EOF) {
			if(c == '\n' && count > max)

			if(c == '\n')
				count = 0;
			count++;

	free(longest_cur);
	free(longest);
}
