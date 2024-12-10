#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

const char err_msg[] = "Unknown signal\n";

volatile static char c = '+';
volatile static sig_atomic_t flag = 0;

void handler(int signum)
{
	signal(signum, handler);
	
	switch(signum) {
		case SIGINT:
			if(flag)
				exit(1);
			else
				flag = 1;
			c = '-';
			alarm(1);
			break;
		case SIGQUIT:
			c = '+';
			alarm(1);
			flag = 0;
			break;
		case SIGALRM:
			flag = 0;
			alarm(1);
			break;
		default:
			write(1, err_msg, sizeof(err_msg) - 1);
			return ;
	}
	
	write(1, (const void*)&c, 1);
}

int main()
{
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	signal(SIGALRM, handler);

	alarm(1);
	while(1) {
		pause();
	}
}
