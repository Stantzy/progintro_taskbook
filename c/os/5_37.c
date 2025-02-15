#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <bits/types/sig_atomic_t.h>

enum { delay = 300 };

const char log_file_name[] = "/tmp/my_daemon.log";

volatile static sig_atomic_t sigusr1_count = 0;
static char *prog_name;
static time_t startup;
static FILE *log;

void print_to_log()
{
    fprintf(
        log,
        "Process: %s, PID=%d, seconds after start =%ld, SIGUSR1 count=%d\n",
        prog_name, getpid(), time(NULL) - startup, sigusr1_count
    );
    fflush(log);
}

void sig_handler(int s)
{
    signal(s, sig_handler);

    if(s == SIGUSR1) 
        sigusr1_count++;

    print_to_log();
    alarm(delay);
}

void daemonize()
{
    int pid;
    close(0);
    close(1);
    close(2);
    open("/dev/null", O_RDONLY);    /* stdin */
    open("/dev/null", O_WRONLY);    /* stdout */
    open("/dev/null", O_WRONLY);    /* stderr */
    chdir("/");

    pid = fork();
    if(pid > 0)
        exit(0);

    setsid();
    pid = fork();
    if(pid > 0)
        exit(0);
}

int main(int argc, char **argv)
{
    daemonize();

    time(&startup);
    prog_name = argv[0];

    log = fopen(log_file_name, "w");
    if(log == NULL) {
        perror("fopen");
        return 1;
    }

    signal(SIGUSR1, sig_handler);
    signal(SIGALRM, sig_handler);

    alarm(delay);
    while(1) {
        pause();
    }

    return 0;
}