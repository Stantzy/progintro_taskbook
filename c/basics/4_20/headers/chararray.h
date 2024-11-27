#ifndef CHARARRAY_H_SENTRY
#define CHARARRAY_H_SENTRY

char *init_char_array(int n);
void read_string_into_array(char **arr, int *flag_eof);
int find_max_word_length(const char *arr);
char *find_longest_digits_pos(const char *arr, int max);
int find_longest_number_length(const char *arr);

#endif