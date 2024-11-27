#include "strings.h"
#include "item.h"

#include <fcntl.h>
#include <unistd.h>

enum { hash_multiplier = 31 };
enum { hashtable_records = 1000000 };

int hash_function(char *name, int table_size)
{
    int hash_value = 0;
    int i;

    for(i = 0; name[i] != '\0'; i++) {
        hash_value =
            (hash_value * hash_multiplier + name[i] ) % table_size;
    }

    return hash_value;
}

int get_file_len(int fd)
{
    return lseek(fd, 0, SEEK_END) / sizeof(struct item_info);
}

int find_item(int fd, struct item_info *item)
{
    int pos, file_len;

    file_len = get_file_len(fd) * sizeof(struct item_info);
    pos = hash_function(item->name, get_file_len(fd));
    lseek(fd, pos, SEEK_SET);

    return pos;
}

int hash_add_item(int fd, struct item_info item)
{


}


