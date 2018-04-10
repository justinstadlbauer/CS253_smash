
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
char *process_token(char *buff);
void exit_smash(char *token, int *token_count);
void process_cd(char *token, int *cd_flag, char *path_buff);
void cd_check(char *token, int *cd_flag, int *token_count);
void echo_input(char *token, int *token_count);
void process_history(char *token, int *token_count);
int exec_program(char *buff[], int percent);

/* --------- GLOBAL VARIABLES --------- */
char *cd_str = "cd"; 									// cd string used for comparison
char *exit_str = "exit"; 								// exit string used for comparison
char *history_str = "history";								// history string used for comparison
int i = 0; 										// Used to format echoed output

int main(void)
{
	srand(time(0));									// Seed rand()
	get_user_input();								// Execute the smash shell
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * *
// Function that implements the smash shell. Four
// built-in shell functions are included:
//  - exit:
//     Exits the shell after the user types "exit"
//  - echo:
//     Echoes user input delimited by space (" ")
//  - cd:
//     Changes present working directory or issues
//     an error if the directory path is invalid 
//  - history:
//     Prints all prior commands issued to the smash
//     shell
// get_user_input() can also execute programs through
// the execve system call 	     
* * * * * * * * * * * * * * * * * * * * * * * * * * */
void get_user_input(void)
{
	init_history(512);				  				// Initialize history structure (allocate memory)

	/* Automatic variables specific to parsing user input */
	char buff[MAXLINE]; 							       	// Buffer to store user input
	char *cmd_args[2048];							       	// Store program command/args (used with execve)
	char *token; 								      	// Pointer to the next token
	char cwd[MAXLINE]; 								     	// Buffer to store path returned by getcwd
	int cd_flag; 								       	// Flag is set if a "cd" token is processed
	int token_count; 							       	// Ensure that "cd" and "exit" execute correctly
	const char *ENV_VAR = getenv("SMASH_KILL");				       	// Environ. var. that determines process exec %
	
	fprintf(stderr,"$ "); 							       	// '$' - marker that characterizes the smash shell
	while (fgets(buff, MAXLINE, stdin) != NULL) 				       	// Process user input from stdin and store in buff
	{
		buff[strlen(buff) - 1] = '\0'; 					       	// Replace newline from fgets with null
		if ((strcmp(&buff[0], "\0") == 0))				       	// "Enter" w/ no input? -> smash doesn't crash
		{
			fprintf(stderr,"$ ");
		}
		else
		{
			add_history(buff);					      	// Add command to smash history
			cd_flag = 0;						       	// Keeps track of the occurence of "cd"
			token = process_token(buff);				       	// Process the next token
			token_count = 0;					      	// Count begins at 0 for each command entered
			while (token != NULL)						// Loop that separates user input into tokens
			{
				if((cd_flag == 1)) 				      	// cd command + path has been entered
				{
					process_cd(token, &cd_flag, cwd);	       	// Implements chdir function 
					break;					      	// Break to process next command
				}				
	       			exit_smash(token, &token_count);  		     	// Exits the shell
				cd_check(token, &cd_flag, &token_count); 	       	// Checks for "cd" token
				process_history(token, &token_count);		    	// Executes the "history" command
//				echo_input(token, &token_count); 		     	// Echoes user input
				cmd_args[token_count] = token;			     	// Store tokens in an array (used with execve)
				token = process_token(NULL); 			     	// Sets up the next token
				token_count++;			  		     	// Increment for each token processed
			}
			if (ENV_VAR != NULL)						// Check if a particular environ. var. is set
			{
				store_exit_status(exec_program(cmd_args, 100));		// Execute all processes
			}
			else
			{
				store_exit_status(exec_program(cmd_args, 75));		// Execute processes 75% of the time
			}
			memset(&cmd_args[0], 0, sizeof(cmd_args));			// Flush buffer used for execve calls
			fprintf(stderr,"$ ");
			i = 0;								// Reset to format echoed input as specified
		}
	}
}

/* Creates tokens from the input string */
char *process_token(char *buff)
{
	return strtok(buff," "); 							// Returns first token delimited by " " in buff 
}

/* Exits the smash shell when the user types "exit" */
void exit_smash(char *token, int *token_count)
{
	if ((strcmp(exit_str, token) == 0) && (*token_count == 0)) 			// Ensures "exit" is the first command
	{
		clear_history(512);							// Before exit: clear history/de-allocate memory
		exit(EXIT_SUCCESS); 							// If "exit", exit the shell
	}
}

/* Echoes input from the user */
void echo_input(char *token, int *token_count)
{
	if ((strcmp(cd_str, token) == 0) && (*token_count == 0)) 			// Ensures the "cd" is the first command
	{
		return;									// Do not echo "cd" if it is the first command
	}
	if ((strcmp(history_str, token) == 0) && (*token_count == 0)) 			// Ensures the "history" is the first command
	{
		return;									// Do not echo "history" if it is the first command
	}
	else 
	{
		printf("[%d] %s\n",i++,token); 						// Echo each token from stdin
	}
}

/* Checks if the first token is "cd" */
void cd_check(char *token, int *cd_flag, int *token_count)
{
	if ((strcmp(cd_str, token) == 0) && (*token_count == 0)) 			// Current token is "cd"
	{
		*cd_flag = 1; 								// Execute cd via next token -> directory path
	}
}

/* Performs the cd operation on the specified directory path */
void process_cd(char *token, int *cd_flag, char *path_buff)
{
	if (chdir(token) == -1) 							// Checks that the directory path is valid
	{
		fprintf(stderr,"error: %s does not exist\n",token);			// Print an error message if invalid directory path
	}
	else 							      			
	{
		printf("%s\n",getcwd(path_buff,MAXLINE));				// Print the present working directory with getcwd
	}
	*cd_flag = 0; 									// Set flag to 0 to process the next cd command
}

/* For a given smash session, prints all prior commands to the smash shell */
void process_history(char *token, int *token_count)
{
	if((strcmp(history_str, token) == 0) && (*token_count == 0))			// Checks that "history" is the first command 
	{
		print_history();							// Prints all prior commands to the smash shell
	}
}

/* Forks a process and executes the specified program in the child process */
int exec_program(char *buff[], int percent)
{
	char *input_cmd;								// Program to execute
	pid_t pid;									// Process ID
	int status;									// Exit status returned by the child
	
	input_cmd = buff[0];

	if ((strcmp(input_cmd, cd_str) == 0) || (strcmp(input_cmd, history_str) == 0))
	{
		return 0;								// Does not create a process for built-in commands
	}
	else if ((rand() % 101) <= percent)						// Executes processes x% of the time - ignores others
	{
		if ((pid = fork()) < 0)							// Fork a process/check for fork errors
		{
			fprintf(stderr,"Fork error\n");					// Provide a msg to the user if fork error 
		}
		else if (pid == 0)							// Child process
		{
			execvp(input_cmd, buff);				 	// Execute program by calling execvp
			fprintf(stderr, "smash: %s: command not found\n", *buff);	// excevp doesn't return -- if it does, print error msg
			exit(EXIT_FAILURE);						// Return -1 to smash
		}
		
		if ((pid = waitpid(pid, &status, 0)) < 0)				// Wait for the child to exit - avoid zombies
		{	
			fprintf(stderr, "waitpid error");				// Provide a msg if waitpid error
		}
	}
	return status;									// Return child exit status to smash
}

