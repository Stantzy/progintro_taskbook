#include <stdio.h>

void print_words_with_brackets(int c, int *t);

int main() {
	int c;
	int t = 0;

	while((c = getchar()) != EOF) {
		print_words_with_brackets(c, &t);
	}
}

int is_start_of_word(int c, int t) {
	return (t == 0 || t == 32 || t == 9) 
		&& (c != 0 && c != 32 && c != 9 && c != 10);
}

int is_char(int c) {
	return (c != '\n' && c != 32 && c != 9);
}

int is_end_of_word(int c, int t) {
	return (c == 0 || c == 32 || c == 9 || c == 10)
	       	&& (t != 0 && t != 32 && t != 9);
}
      
int is_space_or_tab(int c) {
	return (c != '\n') && (c == 32 || c == 9);
}

void print_words_with_brackets(int c, int *t) {
	if(is_start_of_word(c, *t)) {
		putchar('(');
	}
	
	if(is_char(c)) {
		putchar(c);
	}
	
	if(is_end_of_word(c, *t)) {
		putchar(')');
	}

	if(is_space_or_tab(c))  {
		putchar(c);
	}
	
	*t = c;

	if(c == '\n') {
		putchar(c);
		*t = 0;
		c = 0;
	}
}
