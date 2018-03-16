#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "history.h"

#ifndef MAXLINE
#define MAXLINE 4096
#endif

/* FUNCTION PROTOTYPES */
void process_history(char* buff);
struct Cmd* newCmd(void);
void malloc_history(int size);
void free_history(int size);

/* GLOBAL VARIABLES */
struct Cmd* cmd_array[512];
struct Cmd {char* cmd;};
char cmd_input[MAXLINE]; 
char* cpy_cmd;
int n = 0;

void process_history(char* buff)
{
  (void)strncpy(cmd_input,buff,sizeof(cmd_input));
  cpy_cmd = strdup(cmd_input);
  cmd_array[n++]->cmd = cpy_cmd;
  free(cpy_cmd);
}

struct Cmd* newCmd(void)
{
  return (struct Cmd *)malloc(sizeof(struct Cmd));
}

void malloc_history(int size)
{
  for (int j = 0; j < size; j++)
  {
    cmd_array[j] = newCmd();
  }
}

void free_history(int size)
{
  for (int j = 0; j < size; j++)
  {
    free(cmd_array[j]);
  }
}
