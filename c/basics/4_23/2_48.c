#include <stdio.h>

int count_strings(FILE *f)
{
	int c, counter = 0;
	
	while((c = fgetc(f)) != EOF) {
		if(c == '\n')
			counter++;
	}

	return counter;
}

int main(int argc, char **argv)
{
	FILE *f;
	int counter;

	if(argc < 2) {
		fprintf(stderr, "Enter the file\n");
		return 1;
	}

	f = fopen(argv[1], "r");
	if(!f) {
		perror(argv[1]);
		return 2;
	}

	counter = count_strings(f);
	printf("%d\n", counter);
	fclose(f);

	return 0;
}
