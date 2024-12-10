#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

enum { 
	delay = 5,
	max_size = 32
};

volatile sig_atomic_t str_counter = 0;
volatile sig_atomic_t chr_counter = 0;
volatile sig_atomic_t sigint_caught = 0;
volatile sig_atomic_t exit_flag = 0;

const char sleep_msg[] = "Are you there?\n";

void print_counters()
{
	char str_info[max_size];
	char chr_info[max_size];
	int n, m;

	n = snprintf(str_info, sizeof(str_info), "Lines: %d\n", str_counter);
	m = snprintf(chr_info, sizeof(chr_info), "Chars: %d\n", chr_counter);
	
	write(1, str_info, n);
	write(1, chr_info, m);
}

void alrm_hdl(int signum)
{
	signal(SIGALRM, alrm_hdl);
	exit_flag = 0;
	write(1, sleep_msg, sizeof(sleep_msg) - 1);
	alarm(delay);
}

void int_hdl(int signum)
{
	signal(SIGINT, int_hdl);
	sigint_caught = 1;
	
	if(exit_flag)
		exit(1);

	exit_flag = 1;
	print_counters();
	alarm(delay);
}

int main()
{
	char c;
	int read_result = 1;
	
	signal(SIGALRM, alrm_hdl);
	signal(SIGINT, int_hdl);

	while(read_result != 0) {
		alarm(delay);
		read_result = read(0, &c, sizeof(c));
		if(read_result > 0) {
			exit_flag = 0;
			if(c == '\n')
				str_counter++;
			else
				chr_counter++;
		}
	}

	return 0;
}
