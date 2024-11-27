#include <stdio.h>

int get_and_zero(int *pn) {
	int tmp;
	tmp = *pn;
	*pn = 0;
	return tmp;
}

int main() {
	int n = 1, a;
	printf("n = %d\n", n);

	a = get_and_zero(&n);
	printf("a = %d, n = %d\n", a, n);

	return 0;
}
