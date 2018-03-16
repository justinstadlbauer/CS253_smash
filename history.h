
#ifndef HISTORY_H
#define HISTORY_H

void add_history(char* buff);
struct Cmd* newCmd(void);
void init_history(int size);
void clear_history(int size);

#endif /* HISTORY_H */
