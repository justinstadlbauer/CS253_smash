/* --------- INCLUDE STATEMENTS --------- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "history.h"

/* --------- FUNCTION PROTOTYPES --------- */
void add_history(char* buff);
struct Cmd* newCmd(void);
void init_history(int size);
void clear_history(int size);
void print_history(void);

/* --------- GLOBAL VARIABLES --------- */
struct Cmd* cmd_array[512];
struct Cmd {char* cmd; int exit_status;};
int n = 0;
int m = 0;

/* Allocates memory for histoy */
void init_history(int size)
{
  	int j;
  	for (j = 0; j < size; j++)
  	{
    		cmd_array[j] = newCmd();
  	}
}

/* Adds a command to history */
void add_history(char* buff)
{
  	cmd_array[n++]->cmd = strdup(buff);
}

/* Clears history - de-allocates memory */
void clear_history(int size)
{
  int j;
  	for (j = 0; j < size; j++)
  	{
    		free(cmd_array[j]->cmd);
    		free(cmd_array[j]);
  	}
}

/* Prints all prior command history */
void print_history(void)
{
  	int j;
  	for (j = 0; cmd_array[j]->cmd != NULL; j++)
  	{
	  	printf("[%d]  %s\n",j,cmd_array[j]->cmd);
  	}
}

/* Stores process exit status */
void store_exit_status(int status)
{
	cmd_array[m++]->exit_status = status;
}

/* Allocates memory for each struct to store a command/exit status */
struct Cmd* newCmd(void)
{
	return (struct Cmd *)malloc(sizeof(struct Cmd));
}

