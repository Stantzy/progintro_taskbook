#include <stdio.h>

void null_vars(int *a, int *b, int *c, int *d,
		int *e, int *f, int *g, int *h, int *i,
		int *j, int *k, int *l, int *m, int *n, int *o);
int is_end_of_word(int c);
void set_flag_Az(int c, int t, int *fA, int *fz);
void count_words(int c, int t, int *words);
void count_symbols(int c, int *s);
void count_odd_even(int c, int s, int *o, int *e);
void moreseven_lesstwo(int s, int *more, int *less);
void count_A_z(int flag_A, int flag_z, int *word_A_z);
void longest_shortest(int symbols, int *l, int *s);
void count_spaces(int c, int *spaces);
void bracket_balance(int c, int *b);
void brackets_paired(int c, int t, int *p);
void print_answer(int words, int odd, int even, int more_than_seven,
		int less_than_two, int word_Az, int longest, int shortest,
		int longest_seq_of_spaces, int balance_of_brackets,
		int pair_of_brackets);

int main()
{
	int c;
	int symbols = 0; 
	int flag_A = 0, flag_z = 0;
	int words = 0, odd = 0, even = 0;
	int more_than_seven = 0, less_than_two = 0, word_Az = 0;
	int longest = 0, shortest = 0;
	int spaces = 0, longest_seq_of_spaces = 0;
	int balance_of_brackets = 0, pair_of_brackets = 0;
	int t = 0;

	while((c = getchar()) != EOF) {
		count_symbols(c, &symbols);
		count_words(c, t, &words);
		set_flag_Az(c, t, &flag_A, &flag_z);
		count_spaces(c, &spaces);
		bracket_balance(c, &balance_of_brackets);
		brackets_paired(c, t, &pair_of_brackets);

		if(c != 32 && (spaces > longest_seq_of_spaces)) {
			longest_seq_of_spaces = spaces;
			spaces = 0;
		}
		if(is_end_of_word(c)) {
			count_odd_even(c, symbols, &odd, &even);		
			moreseven_lesstwo(symbols, &more_than_seven,
				       	&less_than_two);
			count_A_z(flag_A, flag_z, &word_Az);
			longest_shortest(symbols, &longest, &shortest);
			symbols = 0;
			flag_A = flag_z = 0;
		}
		t = c;
		if(c == '\n') {
			print_answer(words, odd, even, more_than_seven,
					less_than_two, word_Az, longest, shortest, 
					longest_seq_of_spaces,
					balance_of_brackets,
					pair_of_brackets);
			
			null_vars(&words, &symbols, &odd, &even,
				&more_than_seven, &less_than_two, &word_Az,
				&flag_A, &flag_z, &longest, &shortest,
				&spaces, &longest_seq_of_spaces,
				&balance_of_brackets, &pair_of_brackets);
			t = 0;
		}
	}
	return 0;
}

void null_vars(int *a, int *b, int *c, int *d,
		int *e, int *f, int *g, int *h, int *i,
		int *j, int *k, int *l, int *m, int *n, int *o)
{
	*a = *b = *c = *d = *e = 0;
	*f = *g = *h = *i = *j = 0;
	*k = *l = *m = *n = *o = 0;
}

int is_end_of_word(int c)
{
	return c == 32 || c == 9 || c == '\n';
}

int check_A(int c, int t)
{
	return (is_end_of_word(t) || t == 0) && c == 'A';
}

int check_z(int c, int t)
{
	return (is_end_of_word(c)) && t == 'z';
}

void set_flag_Az(int c, int t, int *fA, int *fz)
{
	if(check_A(c, t)) {
		*fA = 1;
	} 
	if(check_z(c, t)) {
		*fz = 1;
	}
}

int is_to_count_words(int c, int t)
{
	return (t != 32 && t != 9 && t != 0) && ((c == '\n') || (c == 32 || c == 9 || c == 0));
}

void count_words(int c, int t, int *words)
{
	if(is_to_count_words(c, t)) {
		(*words)++;
	}
}

void count_symbols(int c, int *s)
{
	if(!(is_end_of_word(c))) {
		(*s)++;
	}
}

int is_to_count_odd_even(int c, int s)
{
	if(s == 0) {
		return 0;
	}
	return s > 0 && ((c == 32 || c == 9 || c == 0) || c == '\n');
}

void count_odd_even(int c, int s, int *o, int *e)
{
	if(is_to_count_odd_even(c, s)) {
		(s % 2 == 0) ? (*e)++ : (*o)++;
	}
}

void moreseven_lesstwo(int s, int *more, int *less)
{
	if(s >= 7) {
		(*more)++;
	}
	if(s <= 2 && s > 0) {
		(*less)++;
	}
}

void count_A_z(int flag_A, int flag_z, int *word_A_z)
{
	if(flag_A && flag_z) {
		(*word_A_z)++;
	}
}

void longest_shortest(int symbols, int *l, int *s)
{

	if(symbols > *l) {
		*l = symbols;
	}
	if(!(*s)) {
		*s = *l;
	}
	if(symbols <= *s) {
		*s = symbols;
	}
}


void count_spaces(int c, int *spaces)
{
	if(c == 32) {
		(*spaces)++;
	}
}


void bracket_balance(int c, int *b)
{
	if(*b < 0) {
		return ;
	}
	if(c == '(') {
		(*b)++;
	}
	if(c == ')') {
		(*b)--;
	}
}


void brackets_paired(int c, int t, int *p)
{
	if(c == ')' && t == '(') {
		(*p)++;
	}
}

void print_answer(int words, int odd, int even, int more_than_seven,
	       int less_than_two, int word_Az, int longest, int shortest,
	       int longest_seq_of_spaces, int balance_of_brackets,
	       int pair_of_brackets)
{
		printf("words: %d\n", words);
		printf("odd: %d, even: %d\n", odd, even);
		printf(">7: %d, <=2: %d\n", 
			more_than_seven, less_than_two);
		printf("A?z words: %d\n", word_Az);
		printf("words: %d, longest word: %d, shortest word: %d\n",
			words, longest, shortest);
		printf("longest word: %d, longest sequence of spaces: %d\n",
			longest,longest_seq_of_spaces);
		balance_of_brackets ? printf("NO\n") : printf("YES\n");
		printf("paired brackets: %d\n", pair_of_brackets);
}
