#include <stdio.h>
#include "generate_random_number.h"

int main()
{
	int random_number;
	int range = 100;

	random_number = generate_random_number(range);

	printf("Random number is %d\n", random_number);

	return 0;
}
