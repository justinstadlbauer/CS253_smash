//----------------------------------------------------------------------------------------
// Justin Stadlbauer
// CS 253
// March 16, 2018
// Dr. Conrad
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// NAME
//  smash - program that implements a basic shell 
//
// SYNOPSIS 
//  smash
//
// DESCRIPTION
//  First part of a larger program that implements a shell. This part includes
//  three features/commands. The first feature performs an exit operation after 
//  the user types "exit." The next feature echoes user input from stdin. Each string 
//  delimited by a space (i.e., " ") is print to the terminal. The final feature 
//  allows the user to change the present working directory. This is accomplished by 
//  issuing the cd command:
//     
//    cd [directory path]
//
//  If the directory path is invalid, an error message is print to the shell. Issuing 
//  the cd command without a specified directory path does not change the present working 
//  directory.
//
// KNOWN BUGS
//  Extra input after the directory path is ignored by the cd command. Need to throw an
//  error.
//
// AUTHORS
//  02/24/2018 ........................................................................jws
//  03/11/2018 ........................................................................jws
//  03/16/2018 ........................................................................jws      
//  04/07/2018 ........................................................................jws
//----------------------------------------------------------------------------------------

/* --------- INCLUDE STATEMENTS --------- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "history.h"

/* --------- DEFINE STATEMENTS --------- */
#define MAXLINE 4096

/* --------- FUNCTION PROTOTYPES --------- */
void get_user_input(void);
char* process_token(char* buff);
void exit_smash(char* token, int* token_count);
void cd_process(char* token, int* cd_flag, char* path_buff);
void cd_check(char* token, int* cd_flag, int* token_count);
void echo_input(char* token, int* token_count);
void process_history(char* token, int* token_count);
int exec_program(char *buff[], int percent);

/* --------- GLOBAL VARIABLES --------- */
char* cd_str = "cd"; 									// Change directory string used for comparison
char* exit_str = "exit"; 								// Exit string used for comparison
char* history_str = "history";
int i = 0; 										// Used to format echoed output

int main(void)
{
	srand(time(0));
	get_user_input();
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * *
// Function that implements three basic features:
//  - exit:
//     Exits the shell after the user types "exit"
//  - echo:
//     Echoes user input delimited by space (" ")
//  - cd:
//     Changes present working directory or issues
//     an error if the directory path is invalid 
* * * * * * * * * * * * * * * * * * * * * * * * * * */
void get_user_input(void)
{
	init_history(512);

	/* Automatic variables specific to parsing user input */
	char buff[MAXLINE]; 									// Buffer to store user input
	char *cmd_args[2048];
	char* token; 										// Pointer to the next token
	char cwd[64]; 								       		// Buffer to store path returned by getcwd
	int cd_flag; 								       		// Flag is set if a "cd" token is processed
	int token_count; 									// Ensure that "cd" and "exit" execute correctly
	const char *ENV_VAR = getenv("SMASH_KILL");						// Environ. var. that determines process exec %
	fprintf(stderr,"$ "); 									// '$' - marker that characterizes the smash shell
	while (fgets(buff, MAXLINE, stdin) != NULL) 						// Process user input from stdin and store in buff
	{
		buff[strlen(buff) - 1] = '\0'; 							// Replace a newline with null
		if ((strcmp(&buff[0], "\0") == 0))
		{
			fprintf(stderr,"$ ");
		}
		else
		{
			add_history(buff);							// Add command to smash history
			cd_flag = 0;
			token = process_token(buff);
			token_count = 0;
			while (token != NULL)
			{
				if((cd_flag == 1)) 						// Performs the "cd" operation
				{
					cd_process(token, &cd_flag, cwd);
					break;
				}				
	       			exit_smash(token, &token_count);  				// Exits the shell
				cd_check(token, &cd_flag, &token_count); 			// Checks for "cd" token
				process_history(token, &token_count);
//				echo_input(token, &token_count); 				// Echoes user input
				cmd_args[token_count] = token;		
				token = process_token(NULL); 					// Sets up the next token
				token_count++;			  
			}
			if (ENV_VAR != NULL)
			{
				store_exit_status(exec_program(cmd_args, 100));
			}
			else
			{
				store_exit_status(exec_program(cmd_args, 75));
			}
			memset(&cmd_args[0], 0, sizeof(cmd_args));
			fprintf(stderr,"$ ");
			i = 0;
		}
	}
}

/* Creates tokens from the input string */
char* process_token(char* buff)
{
	return strtok(buff," "); 							// Assign address of first token delimited by " " in buff 
}

/* Exits the smash shell when the user types "exit" */
void exit_smash(char* token, int* token_count)
{
	if ((strcmp(exit_str, token) == 0) && (*token_count == 0)) 			// Ensures "exit" is the first command
	{
		clear_history(512);
		exit(EXIT_SUCCESS); 							// If "exit", exit the shell
	}
}

/* Echoes input from the user */
void echo_input(char* token, int* token_count)
{
	if ((strcmp(cd_str, token) == 0) && (*token_count == 0)) 			// Ensures the "cd" is the first command
	{
		return;
	}
	if ((strcmp(history_str, token) == 0) && (*token_count == 0)) 			// Ensures the "history" is the first command
	{
		return;
	}
	else 
	{
		printf("[%d] %s\n",i++,token); 						// Echo each token from stdin
	}
}

/* Checks if the first token is "cd" */
void cd_check(char* token, int* cd_flag, int* token_count)
{
	if ((strcmp(cd_str, token) == 0) && (*token_count == 0)) 			// Current token is "cd"
	{
		*cd_flag = 1; 								// Set to 1 to process cd command via the next token - the directory path
	}
}

/* Perform the cd operation on the specified directory path */
void cd_process(char* token, int* cd_flag, char* path_buff)
{
	if (chdir(token) == -1) 							// If the directory path provided is invalid, print an error message
	{
		fprintf(stderr,"error: %s does not exist\n",token);
	}
	else 							      			// If the directory path provided is valid, print the present working directory with getcwd
	{
		printf("%s\n",getcwd(path_buff,64));
	}
	*cd_flag = 0; 									// Set flag to 0 to process next cd command
}

void process_history(char* token, int* token_count)
{
	if((strcmp(history_str, token) == 0) && (*token_count == 0))
	{
		print_history();
	}
}

int exec_program(char *buff[], int percent)
{
	char *input_cmd;
	pid_t pid;
	int status;
	
	input_cmd = buff[0];

	if ((strcmp(input_cmd, cd_str) == 0) || (strcmp(input_cmd, history_str) == 0))
	{
		return 0;
	}
	
	else if ((rand() % 101) <= percent)
	{
		if ((pid = fork()) < 0)
		{
			fprintf(stderr,"Fork error\n");
		}
		else if (pid == 0)
		{
			execvp(input_cmd, buff);
			fprintf(stderr, "smash: %s: command not found\n", *buff);
			exit(EXIT_FAILURE);		
		}

		if ((pid = waitpid(pid, &status, 0)) < 0)
		{	
			fprintf(stderr, "waitpid error");
		}
		else
		{
			return status;
		}
	}
	else
	{	
		return 0;
	}
        return 0;
}

