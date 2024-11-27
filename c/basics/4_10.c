#include <stdio.h>

void print_required_argv(int c, char **v)
{
	if(**v != '-') {
		printf("%s\n", *v);
	}
}

int main(int argc, char **argv)
{
	argv++;		/* skip prog's name */	
	while(*argv) {
		print_required_argv(argc, argv);
		argv++;
	}
	return 0;
}
