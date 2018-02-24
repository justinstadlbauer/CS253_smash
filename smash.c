//--- INCLUDE STATEMENTS ---
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//--- DEFINE STATEMENTS ---
#define MAXLINE 4096

//--- FUNCTION PROTOTYPES ---
void read_user_input(void);

int main(void)
{
  read_user_input();
  return 0;
}

void read_user_input(void)
{
  char buff[MAXLINE];
  char cwd[64];
  char * exit1 = "exit";
  char * cd = "cd";
  char * token;
  int i = 0;
  int cd_flag = 0;
  
  fprintf(stderr,"$");
  while (fgets(buff, MAXLINE, stdin) != NULL)
  {
    buff[strlen(buff) - 1] = '\0';

    token = strtok(buff," ");
    while (token != NULL)
    {
      if (cd_flag == 1)
      {
	if (chdir(token) == -1)
	{
	  fprintf(stderr,"error: %s does not exist\n",token);
	}
	else
	{
	  printf("%s\n",getcwd(cwd,64));
	}
	cd_flag = 0;
	break;
      }
      if (strcmp(exit1,token) == 0)
      {
        exit(EXIT_SUCCESS);
      }
      else if (strcmp(cd,token) == 0)
      {
	cd_flag = 1;
	token = strtok(NULL," ");
      }
      else 
      {
	printf("[%d] %s\n",i++,token);
        token = strtok(NULL," ");
      }    
    }
    i = 0;
    fprintf(stderr,"$");
  }
}

