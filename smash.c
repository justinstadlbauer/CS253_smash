//-----------------------------------------------------------------------------------
// Justin Stadlbauer
// CS 253
// February 24, 2018
// Dr. Conrad
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------

/* ----- INCLUDE STATEMENTS ----- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* ----- DEFINE STATEMENTS ----- */
#define MAXLINE 4096

/* ----- FUNCTION PROTOTYPES ----- */
void read_user_input(void);

/* ----- GLOBAL VARIABLES ----- */
struct Cmd {char *cmd;};
struct Cmd *newCmd(void) {return (struct Cmd *)malloc(sizeof(struct Cmd));};
int n = 0;
int k = 0;
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
  struct Cmd* cmd_array[4096];
  for (int j = 0; j < 4096; j++) // Allocate memory for history
  {
    cmd_array[j] = newCmd();
  }
  char buff[MAXLINE]; // Buffer to store user input
 
  char cmd_input[MAXLINE];
  char cwd[64]; // Buffer to store the absolute path returned by getcwd
  char* exit1 = "exit"; // Exit string used for comparison
  char* cd = "cd"; // Change directory string used for comparison
  char* history = "history";
  char* token; // Pointer to the next token  
  int i = 0; // Used to format echoed output
  int cd_flag = 0; // Flag is set if a "cd" token is processed
  
  fprintf(stderr,"$"); // '$' - marker that characterizes the smash shell
  while (fgets(buff, MAXLINE, stdin) != NULL) // Process user input from stdin and store in buff
  {
    buff[strlen(buff) - 1] = '\0'; // Replace a newline with null
    (void)strncpy(cmd_input,buff,sizeof(cmd_input));
   
    cd_flag = 0; // Ensures that a cd command without a path argument does nothing
    token = strtok(buff," "); // Assign address of first token delimited by " " in buff
    while (token != NULL)
    {
      if ((cd_flag == 1)) // Last token was "cd"
      {
	if (chdir(token) == -1) // If the directory path provided is invalid, print an error message
	{
	  fprintf(stderr,"error: %s does not exist\n",token);
	}
	else // If the directory path provided is valid, print the present working directory with getcwd
	{
	  printf("%s\n",getcwd(cwd,64));
	}
	cd_flag = 0; // Set flag to 0 to process next cd command
	break;
      }
      if (strcmp(history,token) == 0)
      {
        while (cmd_array[k]->cmd != NULL)
        {
          printf("%d %s\n",k,cmd_array[k]->cmd);
          k++;
        }
      }
      if (strcmp(exit1,token) == 0) // Compare present token to "exit"
      {
        exit(EXIT_SUCCESS); // If "exit", exit the shell
      }
      else if (strcmp(cd,token) == 0) // Current token is "cd"
      {
	cd_flag = 1; // Set to 1 to process cd command via the next token - the directory path
	token = strtok(NULL," "); // For each subsequent call to parse user input stored in buff, str is set to NULL
      }
      else
      {
	printf("[%d] %s\n",i++,token); // Echo each token from stdin
        token = strtok(NULL," "); // For each subsequent call to parse user input stored in buff, str is set to NULL
      }    
    }
        
    i = 0;
    cmd_array[n++]->cmd = strdup(cmd_input);
    printf("Input command: %s\n",cmd_input);
    //printf("Address of p: %d\n",(int)p);
    //p++;
    printf("Stored in struct: %s Address: %d\n",cmd_array[0]->cmd,(int)cmd_array[0]);
    printf("Stored in struct: %s Address: %d\n",cmd_array[1]->cmd,(int)cmd_array[1]);
    printf("Stored in struct: %s Address: %d\n",cmd_array[2]->cmd,(int)cmd_array[2]);
    printf("n: %d\n",n);
    fprintf(stderr,"$");
  }
  for (k = 0; k < 4095; k++)
  {
    free(cmd_array[k]->cmd);
  }
}

