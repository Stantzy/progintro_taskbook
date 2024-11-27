#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

enum { cmd_max_len = 64, list_max_len = 64 };

/*
int get_command_list(char **dest, int argc, char **argv)
{
	int i, cur_pos;
	char cmd[cmd_max_len];

	for(i = 1, cur_pos = 0; i < argc; i++) {
		if(*(argv[i]) == ';')
			continue;
		dest[cur_pos] = argv[i];
		cur_pos++;
	}

	dest[cur_pos] = NULL;
	return cur_pos;
}
*/

struct proc_info {
	char *prog_name;
	char **params;
	int pid;
};

void execute_command(struct proc_info *prog)
{
	execvp(prog->prog_name, prog->params);
	perror(prog->prog_name);
	exit(1);
}

char *get_command(char **argv)
{
	char *cmd[cmd_max_len];
	int i;

	for(i = 0; !strcmp(argv[i], NULL) && !strcmp(argv[i], ";;"); i++) {
		strcpy(cmd[i], argv[i]);
	}
	cmd[i] = NULL;
	
	return cmd;
}

struct proc_info *get_command_list(int argc, char **argv)
{
	char **cmd;
	struct proc_info *first = NULL, *tmp = NULL;
	int i;

	cmd = get_command(argv);

	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i], ";;"))
			continue;
		tmp = malloc(sizeof(struct proc_info));
		
	}
}

int main(int argc, char **argv)
{
	int wr, status, pid;
	int i;
	struct proc_info b;

	if(argc < 2) {
		fprintf(stderr, "Provide at least one prog to run\n");
		exit(-1);
	}

	for(i = 1; i < argc;) {

		

		pid = fork();
		if(pid == -1) {
			perror("pid");
			return 1;
		}
		if(pid == 0) {
			execute_command();
		}
	}

	while((wr = wait(&status)) != -1) {
		printf("Process with pid=%d finished.\n", wr);
		if(WIFEXITED(status)) {
			printf("It has exited with code=%d\n",
				WEXITSTATUS(status));
		}	
	}

	exit(0);
}
