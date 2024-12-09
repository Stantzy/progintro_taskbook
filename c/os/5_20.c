#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

const char message[] = "Good bye!\n";

void handler()
{
	write(1, message, sizeof(message) - 1);
	exit(1);
}

int main()
{
	signal(SIGINT, handler);
	pause();
	return 0;
}
