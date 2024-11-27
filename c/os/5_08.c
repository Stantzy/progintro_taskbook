#include <stdio.h>
#include <string.h>
#include <dirent.h>

enum { max_name_size = 256 };

void print_file_list(DIR *dir, const char *base_path)
{
	DIR *new_dir;
	struct dirent *dent;
	char path[max_name_size];
	char new_path[max_name_size];
	char file[max_name_size];
	int cut_len = 0;

	strcpy(path, base_path);
	strcat(path, "/");
	strcpy(file, path);
	cut_len = strlen(path);

	while((dent = readdir(dir)) != NULL) {
		strcat(file, dent->d_name);
		new_dir = opendir(file);

		if(*(dent->d_name) != '.' && !new_dir) {
			printf("%s", path);
			printf("%s\n", dent->d_name);
		}

		if(new_dir && *(dent->d_name) != '.') {
			strcpy(new_path, path);
			strcat(new_path, dent->d_name);
			print_file_list(new_dir, new_path);
		}

		closedir(new_dir);
		strncpy(file, path, cut_len);
		file[cut_len] = '\0';
	}
}

int main(int argc, char **argv)
{
	DIR *dir;
	const char *pathname = ".";

	if(argc > 1)
		pathname = argv[1];

	dir = opendir(pathname);
	if(!dir) {
		perror(pathname);
		return 1;
	}

	print_file_list(dir, pathname);

	closedir(dir);
	return 0;
}
