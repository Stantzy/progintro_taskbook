#include <stdio.h>

int is_to_print_word(int s, int c);
int is_to_print_end(int s, int c);
int is_EOL(int s, int c);

int main() {
	int c;
	int t_1 = 0, t_2 = 0;
	int symbols = 0;

	while((c = getchar()) != EOF) {
		symbols++;	
		if(is_to_print_word(symbols, c)) {
			putchar(t_2);
			putchar(t_1);
		}
		if(is_to_print_end(symbols, c)) {
			putchar(c);
		}
		if(is_EOL(symbols, c)) {
			symbols = t_1 = t_2 = c = 0;
		}
		
		if(t_1 != 0) {
			t_2 = t_1;
		}
		t_1 = c;
	}
}

int is_to_print_word(int s, int c) {
	return s == 3 && (c == 9 || c == 32 || c == 10);
}

int is_to_print_end(int s, int c) {
	return (c == 9 || c == 32 || c == 10) && (s == 3 || c == 10);
}

int is_EOL(int s, int c) {
	return (c == 9 || c == 32 || c == 10) && (s == 3 || c != 10);
}
