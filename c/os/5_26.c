#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

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
		//if(tmp->word != NULL)
		//	free(&(tmp->word));
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

int main(int argc, char **argv)
{
	int result = 0;
	int fd[2];		/* fd[0] for reading, fd[1] for writing */
	int pid_1, pid_2;
	char **cur_pos_argv;
	char **proc_argv;
	int item_count = 0;
	struct prog_item *first_prog = NULL;
	struct prog_item *second_prog = NULL;

	if(argc < 4) {
		fprintf(
			stderr,
			"Provide names of two progs separated by ';;'\n"
			);
		return 1;
	}

	first_prog = init_prog_item();
	second_prog = init_prog_item();
	cur_pos_argv = fill_prog_info(first_prog, argv + 1);
	cur_pos_argv = fill_prog_info(second_prog, cur_pos_argv);
	
	pipe(fd);

	pid_1 = fork();
	if(pid_1 == -1) {
		perror("pid_1");
		result = 1;
		goto end;
	}
	if(pid_1 == 0) {		/* child #1 */
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);

		item_count = count_prog_items(first_prog);
		proc_argv = malloc(sizeof(char *) * item_count);
		copy_ptrs_to_arr(first_prog, proc_argv);

		execvp(*proc_argv, proc_argv);
		perror(*proc_argv);
		/* write release memory */
		exit(1);
	}

	pid_2 = fork();
	if(pid_2 == -1) {
		perror("pid_2");
		result = 1;
		goto end;
	}
	if(pid_2 == 0) {		/* child #2 */
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);

		item_count = count_prog_items(second_prog);
		proc_argv = malloc(sizeof(char *) * item_count);
		copy_ptrs_to_arr(second_prog, proc_argv);
		
		execvp(*proc_argv, proc_argv);
		perror(*proc_argv);
		/* write release memory */
		exit(1);
	}

	/* parent */
	close(fd[0]);
	close(fd[1]);

	while(wait(NULL) != -1);

end:	release_memory(first_prog);
	release_memory(second_prog);

	return result;
}
