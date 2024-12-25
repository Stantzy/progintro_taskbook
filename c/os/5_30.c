#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define B

void check_argc(int argc)
{
	if(argc < 4) {
		fprintf(stderr, "Provide two progs separated by ';;'\n");
		exit(1);
	}
}

struct prog_item {
	char *word;
	struct prog_item *next;
};

struct prog_item *init_prog_item()
{
	struct prog_item *item;
	item = malloc(sizeof(struct prog_item));
	item->word = NULL;
	item->next = NULL;
	return item;
}

char **fill_prog_info(struct prog_item *head, char **argv)
{
	struct prog_item *tmp = head;

	if((strcmp(*argv, ";;") == 0))
		argv++;

	while(*argv != NULL && (strcmp(*argv, ";;") != 0)) {
		tmp->word = malloc(sizeof(char *));
		tmp->word = *argv;
		tmp->next = init_prog_item();
		tmp = tmp->next;
		argv++;
	}

	return argv;
}

int count_prog_items(struct prog_item *head)
{
	int counter = 0;
	struct prog_item *tmp = head;

	while(tmp != NULL) {
		counter++;
		tmp = tmp->next;
	}

	return counter;
}

void release_memory(struct prog_item *head)
{
	struct prog_item *tmp = head;

	while(tmp != NULL) {
		head = tmp->next;
		free(tmp);
		tmp = head;
	}
}

void copy_ptrs_to_arr(struct prog_item *head, char **ptrs)
{
	struct prog_item *tmp_item = head;
	char **tmp_p = ptrs;

	while(tmp_item != NULL) {
		*tmp_p = tmp_item->word;
		tmp_p++;
		tmp_item = tmp_item->next;
	}
}

void filter_io(int fd_in, int fd_out)
{
	FILE *in, *out;
	char c;
	int flag_out = 0;

	in = fdopen(fd_in, "r");
	out = fdopen(fd_out, "w");

	while((c = getc(in)) != EOF) {
#if defined(A)	
		if(flag_out)
			putc(c, out);
		if(c == '\n')
			flag_out = !flag_out;
#elif defined(B)
		if((c == '\t' || c == ' ') && (flag_out == 0))
			flag_out = 1;
		else if(flag_out == 0)
			flag_out = -1;
		if(flag_out == 1)
			putc(c, out);
		if(c == '\n')
			flag_out = 0;
#elif defined (C)
		if(flag_out < 10 || c == '\n')
			putc(c, out);
		if(c == '\n')
			flag_out = 0;
		else
			flag_out++;
#endif
	}

	fclose(in);
	fclose(out);
}

int main(int argc, char **argv)
{
	int pid_1, pid_2;
	int fd_1[2], fd_2[2];
	char **cur_pos_argv;
	char **proc_argv;
	int item_count = 0;
	struct prog_item *first_prog = NULL;
	struct prog_item *second_prog = NULL;

	check_argc(argc);

	first_prog = init_prog_item();
	second_prog = init_prog_item();
	cur_pos_argv = fill_prog_info(first_prog, argv + 1);
	cur_pos_argv = fill_prog_info(second_prog, cur_pos_argv);

	pipe(fd_1);
	pipe(fd_2);

	pid_1 = fork();
	if(pid_1 == -1) {
		perror("Fork");
		return 2;
	}
	if(pid_1 == 0) {
		close(fd_2[0]);
		close(fd_2[1]);

		close(fd_1[0]);
		dup2(fd_1[1], 1);
		close(fd_1[1]);

		item_count = count_prog_items(first_prog);
		proc_argv = malloc(sizeof(char *) * item_count);
		copy_ptrs_to_arr(first_prog, proc_argv);
		
		execvp(*proc_argv, proc_argv);
		perror(*proc_argv);
		_exit(1);

	}

	pid_2 = fork();
	if(pid_2 == -1) {
		perror("Fork");
		return 3;
	}
	if(pid_2 == 0) {
		close(fd_1[0]);
		close(fd_1[1]);

		close(fd_2[1]);
		dup2(fd_2[0], 0);
		close(fd_2[0]);

		item_count = count_prog_items(second_prog);
		proc_argv = malloc(sizeof(char *) * item_count);
		copy_ptrs_to_arr(second_prog, proc_argv);

		execvp(*proc_argv, proc_argv);
		perror(*proc_argv);
		_exit(1);
	}

	close(fd_1[1]);
	close(fd_2[0]);

	filter_io(fd_1[0], fd_2[1]);

	close(fd_1[0]);
	close(fd_2[1]);
	
	release_memory(first_prog);
	release_memory(second_prog);

	while(wait(NULL) != -1) {}

	return 0;
}
