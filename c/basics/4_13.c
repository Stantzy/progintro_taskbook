#include <stdio.h>



void put_square_root_in_str(char *str, long *n, long long *n_sq)
{
	*n_sq += *n + *n + 1;
	(*n)++;
	sprintf(str, "%lld", *n_sq);
}

void print_array_of_square_roots(int start, int end)
{
	enum { buffer = 20 };
	char square[buffer];
	const char *p_square;
	
	long n = 0;
	long long n_sq = 0;
	int index = 0;
	for(;;) {
		put_square_root_in_str(square, &n, &n_sq);
		for(p_square = square; *p_square; p_square++, index++) {
			if(index >= end)
				goto fin;
			else if(index >= start)
				putchar(*p_square);
		}
	}

fin:	putchar('\n');

}

int main(int argc, char **argv)
{
	int start, end;

	if(argc != 3) {
			fprintf(stderr, "Enter two args: start and end index.\n");
			return 1;
	}

	sscanf(argv[1], "%d", &start);
	sscanf(argv[2], "%d", &end);
	start--;
	print_array_of_square_roots(start, end);
	
	return 0;
}
