#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

volatile static sig_atomic_t n = 0;
volatile static sig_atomic_t flag = 0;

void sigint_hdl(int s)
{
	signal(SIGINT, sigint_hdl);
	n++;
}

void sigusr1_hdl(int s)
{
	signal(SIGUSR1, sigusr1_hdl);
	flag = 1;
}

int main()
{
	signal(SIGINT, sigint_hdl);
	signal(SIGUSR1, sigusr1_hdl);

	while(1) {
		pause();
		if(flag) {
			printf("%d\n", n);
			flag = 0;
		}
	}

	return 0;
}
