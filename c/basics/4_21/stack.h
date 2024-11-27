#ifndef STACK_H_SENTRY
#define STACK_H_SENTRY

struct stackdbl;
struct stackdbl *stackdbl_init(int start_buffer);
void stackdbl_destroy(struct stackdbl *p_stack);
int stackdbl_empty(struct stackdbl *p_stack);
int stackdbl_push(struct stackdbl *p_stack, double x);
double stackdbl_pop(struct stackdbl *p_stack);

#endif