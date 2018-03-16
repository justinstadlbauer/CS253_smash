#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "history.h"

/* FUNCTION PROTOTYPES */
void add_history(char* buff);
struct Cmd* newCmd(void);
void init_history(int size);
void clear_history(int size);
void print_history(void);

/* GLOBAL VARIABLES */
struct Cmd* cmd_array[512];
struct Cmd {char* cmd;};
int n = 0;

void init_history(int size)
{
  for (int j = 0; j < size; j++)
  {
    cmd_array[j] = newCmd();
  }
}

void add_history(char* buff)
{
  cmd_array[n++]->cmd = strdup(buff);
}

void clear_history(int size)
{
  for (int j = 0; j < size; j++)
  {
    free(cmd_array[j]->cmd);
    free(cmd_array[j]);
  }
}

void print_history(void)
{
  for (int i = 0; cmd_array[i]->cmd != NULL; i++)
  {
    printf("[%d]  %s\n",i,cmd_array[i]->cmd);
  }
}

struct Cmd* newCmd(void)
{
  return (struct Cmd *)malloc(sizeof(struct Cmd));
}

