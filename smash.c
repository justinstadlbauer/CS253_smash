//----------------------------------------------------------------------------------------
// Justin Stadlbauer
// CS 253
// March 11, 2018
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
//----------------------------------------------------------------------------------------

/* --------- INCLUDE STATEMENTS --------- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "history.h"

/* --------- DEFINE STATEMENTS --------- */
#define MAXLINE 4096

/* --------- FUNCTION PROTOTYPES --------- */
void read_user_input(void);
char* process_token(char* buff);
void exit_smash(char* token, int* token_count);
void cd_process(char* token, int* cd_flag, char* path_buff);
void cd_check(char* token, int* cd_flag, int* token_count);
void echo_input(char* token, int* token_count);

/* --------- GLOBAL VARIABLES --------- */
char* cd_str = "cd"; // Change directory string used for comparison
char* exit_str = "exit"; // Exit string used for comparison
int i = 0; // Used to format echoed output

int main(void)
{
  
  read_user_input();
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
void read_user_input(void)
{
  malloc_history(512);
  
  /* Automatic variables specific to parsing user input */
  char buff[MAXLINE]; // Buffer to store user input
  char* token; // Pointer to the next token
  char cwd[64]; // Buffer to store the absolute path returned by getcwd
  int cd_flag; // Flag is set if a "cd" token is processed
  int token_count; // Keeps track of number of tokens - used to ensure that "cd" and "exit" are executed correctly

  fprintf(stderr,"$"); // '$' - marker that characterizes the smash shell
  while (fgets(buff, MAXLINE, stdin) != NULL) // Process user input from stdin and store in buff
  {
    buff[strlen(buff) - 1] = '\0'; // Replace a newline with null

    process_history(buff);

    cd_flag = 0;
    token = process_token(buff);
    token_count = 0;
    while (token != NULL)
    {
      if((cd_flag == 1)) // Performs the "cd" operation
      {
        cd_process(token, &cd_flag, cwd);
        break;
      }
      exit_smash(token, &token_count);  // Exits the shell
      cd_check(token, &cd_flag, &token_count); // Checks for "cd" token
      echo_input(token, &token_count); // Echoes user input
      token = process_token(NULL); // Sets up the next token
      token_count++;
    }
    fprintf(stderr,"$");
    i = 0;
  }
}

/* Creates tokens from the input string */
char* process_token(char* buff)
{
  return strtok(buff," "); // Assign address of first token delimited by " " in buff   
}

/* Exits the smash shell when the user types "exit" */
void exit_smash(char* token, int* token_count)
{
  if ((strcmp(exit_str, token) == 0) && (*token_count == 0)) // Ensures "exit" is the first command
  {
    free_history(512);
    exit(EXIT_SUCCESS); // If "exit", exit the shell
  }
}

/* Echoes input from the user */
void echo_input(char* token, int* token_count)
{
  if ((strcmp(cd_str, token) == 0) && (*token_count == 0)) // Ensures the "cd" is the first command
  {
    return;
  }
  else
  {
    printf("[%d] %s\n",i++,token); // Echo each token from stdin
  }
}

/* Checks if the first token is "cd" */
void cd_check(char* token, int* cd_flag, int* token_count)
{
  if ((strcmp(cd_str, token) == 0) && (*token_count == 0)) // Current token is "cd"
  {
    *cd_flag = 1; // Set to 1 to process cd command via the next token - the directory path
  }
}

/* Perform the cd operation on the specified directory path */
void cd_process(char* token, int* cd_flag, char* path_buff)
{
  if (chdir(token) == -1) // If the directory path provided is invalid, print an error message
  {
    fprintf(stderr,"error: %s does not exist\n",token);
  }
  else // If the directory path provided is valid, print the present working directory with getcwd
  {
    printf("%s\n",getcwd(path_buff,64));
  }
  *cd_flag = 0; // Set flag to 0 to process next cd command
}

