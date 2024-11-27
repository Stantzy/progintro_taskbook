#ifndef TABLE_H_SENTRY
#define TABLE_H_SENTRY

enum { name_size = 60 };

struct item_info {
	char name[name_size];  
	int count;
};

void copy_itemname(char *dest, char *src);
int add_item(int fd, struct item_info *item);
int output_item(int fd, struct item_info *item);
int output_table(int fd);
int join_tables(int int_fd1, int int_fd2, int out_fd);

#endif /* TABLE_H_SENTRY */