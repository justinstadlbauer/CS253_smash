 Justin Stadlbauer
 CS 253 / smash README
 April 8, 2018
 Dr. Conrad

 NAME
  smash - program that implements a basic shell 

 SYNOPSIS 
  smash

 DESCRIPTION
  Smash implements a command-line interface -- similar to bash. Four built-in features
  characterize the smash shell. The first feature performs an exit operation after 
  the user types "exit." The next feature echoes user input from stdin. Each string 
  delimited by a space (i.e., " ") is print to the terminal. In order to provide a more
  familiar command-line experience, this version of smash does not implement the echo
  feature. The user can activate the echo feature by un-commenting line 90 in smash.c and
  re-compiling smash with the provided Makefile. The third feature allows the user to change
  the present working directory. This is accomplished by issuing the cd command:
     
    cd [directory path]

  If the directory path is invalid, an error message is print to the shell. Issuing 
  the cd command without a specified directory path does not change the present working 
  directory. The final built-in command utilizes the heap to store all prior commands to
  the smash shell (for a given session). After appropriately allocating memory, each command
  is stored in struct. Each struct is then sequentially placed in an array. Typing "history"
  accesses this array and abstracts the stored commands. After smash exits, memory allocated
  is freed to avoid memory leaks.

  Smash can also execute programs by implementing the execve system call (with execvp). For
  each command that is not a built-in command, smash forks a process. The child process attempts
  to execute the program associated with the command. If the program given by the command is not
  valid, execve returns and an error message is output to the user. To avoid creating zombie
  processes, the waitpid function is implemented. This ensures that the parent retrieves the child's
  exit status. The exit status is placed in the history struct associated with the command and can
  be accessed by modifying history.c.

 KNOWN BUGS
  Extra input after the directory path is ignored by the cd command. Need to throw an
  error. On Arch Linux, attempting to cd into certain directories (directories for other
  C programs) causes a seg-fault.

 MISC.
  This program was written on a personal linux laptop. Gedit and Emacs were used to
  write smash. GDB functionality in Emacs was used to debug the program as required. A
  personal remote respository (www.github.com/justinstadlbauer) was used to utilize
  version control (git). Every significant change to the program was pushed to github.

  While the program functions as expected, a more elegant approach can be implemented
  to process user input. A separate function can be created to store user input (fgets),
  process tokens (strtok), and store tokens in an array. This array can be used to parse
  user input and implement all features of the smash shell. These changes require that the
  program be re-structured and will be implemented in the future.

AUTHOR
 02/24/2018 ........................................................................jws
 03/11/2018 ........................................................................jws
 03/16/2018 ........................................................................jws      
 04/07/2018 ........................................................................jws
