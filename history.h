
#ifndef HISTORY_H
#define HISTORY_H

void add_history(char* buff);
void init_history(int size);
void clear_history(int size);
void print_history(void);
void store_exit_status(int status);

#endif /* HISTORY_H */
