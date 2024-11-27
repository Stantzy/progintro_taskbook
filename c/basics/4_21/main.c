#include "stack.h"
#include <stdio.h>

int main()
{
	enum { start_buffer = 12 };
	struct stackdbl *st;
	double data, tmp;
	
	st = stackdbl_init(start_buffer);
	if(st == NULL) {
		fputs("Memory allocation error.", stderr);
		return 1;
	}

	while(scanf("%lf", &data) == 1) {
		stackdbl_push(st, data);
	}

	while(!stackdbl_empty(st)) {
		tmp = stackdbl_pop(st);
		printf("%lf\n", tmp);
	}

	stackdbl_destroy(st);

	return 0;
}
