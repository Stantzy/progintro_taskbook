#include <stdio.h>

int read_to_file(char *filename)
{
	FILE *dest;
	int c;

	dest = fopen(filename, "w");
	if(!dest) {
		perror(filename);
		return 1;
	}

	while((c = getchar()) != EOF)
		fputc(c, dest);

	fclose(dest);

	return 0;
}

int main(int argc, char **argv)
{	
	int error_flag = 0;

	if(argc < 2) {
		fprintf(stderr, "Enter the file to be recorded\n");
		return 1;
	}

	error_flag = read_to_file(argv[1]);
	if(error_flag) {
		perror(argv[1]);
		return 2;
	}

	return 0;
}
