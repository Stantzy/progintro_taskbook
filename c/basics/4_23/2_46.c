#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *src, *dest;
	int c;

	if(argc < 2) {
		fprintf(stderr, "Enter the file name to record\n");
		return 1;
	}

	src = fopen("humptydumpty.txt", "r");
	if(!src) {
		perror("humptydumpty.txt");
		return 2;
	}

	dest = fopen(argv[1], "w");
	if(!dest) {
		perror(argv[1]);
		return 3;
	}
	
	while((c = fgetc(src)) != EOF)
		fputc(c, dest);

	fclose(src);
	fclose(dest);

	return 0;
}
