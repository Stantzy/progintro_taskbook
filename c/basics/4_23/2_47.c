#include <stdio.h>

void print_file(FILE *f)
{
	int c;

	while((c = fgetc(f)) != EOF)
		putchar(c);
}

int main(int argc, char **argv)
{
	FILE *src;

	if(argc < 2) {
		fprintf(stderr, "Define the file name\n");
		return 1;
	}

	src = fopen(argv[1], "r");
	if(!src) {
		perror(argv[1]);
		return 2;
	}

	print_file(src);
	fclose(src);

	return 0;
}
