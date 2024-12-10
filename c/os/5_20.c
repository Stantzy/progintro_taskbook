#include <signal.h>
#include <unistd.h>
#include <stdio.h>

volatile int sig_caught = 0;

void handler(int s)
{
	sig_caught = 1;
}

int main()
{
	signal(SIGINT, handler);
	printf("Press Ctrl-C to quit\n");

	while(!sig_caught)
		pause();

	printf("Good bye\n");

	return 0;
}
