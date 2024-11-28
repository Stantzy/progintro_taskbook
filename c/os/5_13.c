#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

enum { name_max_len = 64 };

struct proc_info {
	char *prog_name;
	char **args;
	int pid;
};

void execute_command(struct proc_info *pc)
{
	execvp(pc->prog_name, pc->args);
	perror(pc->prog_name);
	exit(1);
}

int get_command(char **argv, char **cmd)
{
	int i;

	for(i = 0; argv[i] != NULL && strcmp(argv[i], ";;") != 0; i++) {
		cmd[i] = argv[i];
	}
	cmd[i] = NULL;
	
	return i;
}

struct proc_info *get_prog_by_pid(int wr, struct proc_info **pptr)
{
	while(*pptr != NULL) {
		if((*pptr)->pid == wr) {
			return *pptr;
		}
		pptr++;
	}

	return NULL;
}

void release_memory(struct proc_info **pptr)
{
	struct proc_info **tmp = pptr;

	while(*tmp != NULL) {
		free(*tmp);
		tmp++;
	}
	free(pptr);
}

int main(int argc, char **argv)
{
	int wr, status, pid;
	int offset;
	int i;
	struct proc_info **pptr;
	struct proc_info **ptr_pc;
	struct proc_info *pc;
	char *cmd[name_max_len];

	if(argc < 2) {
		fprintf(stderr, "Provide at least one prog to run\n");
		exit(-1);
	}

	ptr_pc = malloc((argc / 2) * sizeof(struct proc_info *));
	pptr = ptr_pc;

	for(i = 1; i < argc;) {
		if((offset = get_command(&(argv[i]), cmd)) != 0) {
			i += offset;
			pc = malloc(sizeof(struct proc_info));
			pc->prog_name = cmd[0];
			pc->args = &(cmd[0]);
			*pptr = pc;
			pptr++;
		} else {
			i++;
			continue;
		}

		pid = fork();
		if(pid == -1) {
			perror("pid");
			return 1;
		}
		if(pid == 0) {
			execute_command(pc);
		}
		pc->pid = pid;
	}

	*pptr = NULL;
	pptr = ptr_pc;

	while((wr = wait(&status)) != -1) {
		if(WIFEXITED(status)) {
			pc = get_prog_by_pid(wr, pptr);
			printf(
				"Program \"%s\" with pid=%d successfully completed.\n",
				pc->prog_name, wr
				);
		}
	}	

	release_memory(ptr_pc);
	exit(0);
}
