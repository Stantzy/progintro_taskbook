#include <stdio.h>

int put_sum(int *pa, int *pb, int *pc) {
	int res;
	
	res = *pa + *pb + *pc;
	
	*pa = res;
	*pb = res;
	*pc = res;

	return res;
}

int main() {
	int a = 1, b = 1, c = 1;
	printf("a=%d, b=%d, c=%d\n", a, b, c);

	put_sum(&a, &b, &c);
	printf("a=%d, b=%d, c=%d\n", a, b, c);

	return 0;
}
