#include <stdlib.h>
#include <time.h>

int generate_random_number(int range)
{
	srand(time(NULL));	
	return rand() % range;
}
