
#ifndef HISTORY_H
#define HISTORY_H

void process_history(char* buff);
struct Cmd* newCmd(void);
void malloc_history(int size);
void free_history(int size);

#endif /* HISTORY_H */
