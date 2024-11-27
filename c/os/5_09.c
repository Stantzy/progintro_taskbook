#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

void print_basic_info(struct stat st)
{
	printf("ID of containing device: %lx\n", st.st_dev);	
	printf("ID: %ju\n", st.st_ino);
	printf("File type: ");
	switch(st.st_mode & S_IFMT) {
		case S_IFSOCK:
			printf("Socket\n");
			break;
		case S_IFLNK:
			printf("Symbolic link\n");
			break;
		case S_IFREG:
			printf("Regular file\n");
			break;
		case S_IFBLK:
			printf("Block device\n");
			break;
		case S_IFDIR:
			printf("Directory\n");
			break;
		case S_IFCHR:
			printf("Character device\n");
			break;
		case S_IFIFO:
			printf("FIFO\n");
			break;
		default:
			printf("Unknown device\n");
			break;
	}
	printf("Number of hard links: %d\n", (int)st.st_nlink);
	printf("Ownership: UID=%ju   GID=%ju\n",
			(long int)st.st_uid, (long int)st.st_gid);
	printf("Total size: %jd\n", st.st_size);
	printf("Block size for filesystem I/O: %jd\n", st.st_blksize);
	printf("Blocks allocated: %jd\n", st.st_blocks);
}

void print_time_info(struct stat st)
{
	printf("Time of last access: %s", ctime(&st.st_atime));
	printf("Time of last modification: %s", ctime(&st.st_mtime));
	printf("Time of last status change: %s", ctime(&st.st_ctime));

}

void print_permission_info(struct stat st)
{
	enum { perms_len = 9 };
	enum { x = 1, w = 2, r = 3 };
	char perms[perms_len] = {"---------"};
	int i, check, pos_counter;

	for(
		i = 0, check = 1, pos_counter = 1;
		i < perms_len;
		i++, check <<= 1, pos_counter++
	) {
		if(st.st_mode & check) {
			switch(pos_counter) {
			case x:
				perms[i] = 'x';
				break;
			case w:
				perms[i] = 'w';
				break;
			case r:
				perms[i] = 'r';
				pos_counter = 0;
				break;
			}
		}
	}

	printf("Permissions: ");
	for(i = perms_len - 1; i >= 0; i--)
		printf("%c", perms[i]);

	printf("\n");
	printf("set-uid: %s, set-gid: %s, sticky-bit: %s\n",
		st.st_mode & S_ISUID ? "on" : "off",
		st.st_mode & S_ISGID ? "on" : "off",
		st.st_mode & S_ISVTX ? "on" : "off"
	      );
}

void print_file_info(struct stat st)
{
	print_basic_info(st);
	print_permission_info(st);
	print_time_info(st);
}

int main(int argc, char **argv)
{
	struct stat st;
	int res;

	if(argc != 2) {
		fprintf(stderr, "Specify the file name\n");
		return 1;
	}

	if(lstat(argv[1], &st) == -1) {
		perror("stat");
		return 2;
	}
	
	print_file_info(st);

	if((st.st_mode & S_IFMT) == S_IFLNK) {
		printf("\n");
		if(stat(argv[1], &st) == -1)
			printf("Dangling\n");
		else
			print_file_info(st);
	}

	return 0;
}
