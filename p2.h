#include "getword.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAXSIZE 100					// Maximum words per line 
#define MAXSTORAGE 25600			// Maximum storage for the big buffer (each word can be up to 255 characters)
#define MAXPIPELINE 10				// Maximum number of pipelines

// Exit status
#define EXIT_PROGRAM_SUCCESS 0
#define EXIT_PROGRAM_FAILURE -1
#define UNSUCCESSFUL_FORK -2
#define INPUT_REDIRECTION_AMBIGUITY -3
#define OUTPUT_REDIRECTION_AMBIGUITY -4
#define READ_FILE_FAILURE -5
#define CREATE_FILE_FAILURE -6
#define INPUT_REDIRECTION_NAME_UNKNOWN -7
#define OUTPUT_REDIRECTION_NAME_UNKNOWN -8
#define DUPLICATE_STDOUT_FAILURE -9
#define DUPLICATE_STDIN_FAILURE -10
#define DUPLICATE_NULL_DEVICE_FAILURE -11
#define READ_NULL_DEVICE_FAILURE -12
#define GENERAL_REDIRECTION_FAILURE -13
#define CREATE_PIPE_FAILURE -14
#define COMMAND_BEFORE_PIPE_NOT_FOUND -15
#define COMMAND_AFTER_PIPE_NOT_FOUND -16
#define TOO_MANY_PIPELINE -17
#define INVALID_NULL_COMMAND -18
#define UNDEFINED_VARIABLE -19
#define UNKNOWN_USER -20
#define COMMAND_NOT_FOUND 127

// Prototype

/*************************************************************************
* FUNCTION parse
**************************************************************************
* 	This function is responsible for the syntactical analysis. It will
* read the input from the command line and tokenize that line into single
* words. In addition, it will also turn the specific flag upon seeing
* metacharacter
*************************************************************************/
int parse(char *argv, char **newargv);


/*************************************************************************
* FUNCTION handler 
**************************************************************************
* 	This function helps the main function to catch the sigterm (when this
* program is running in the background), allowing the program to exit 
* gracefully.
*************************************************************************/
void handler();


/*************************************************************************
* FUNCTION redirection 
**************************************************************************
* 	This function sets up (both or either) the input, output redirection
* while processing the command line
*************************************************************************/
int redirection();


/*************************************************************************
* FUNCTION execute 
**************************************************************************
* 	This function executes the command line in the child process (and 
* grandchild if there's one), vertical piping is also handle inside this
* function
*************************************************************************/
void execute(char *command, char **arguments);


/*************************************************************************
* FUNCTION reset 
**************************************************************************
* This function reinitialize all of the flags to avoid any glitches, bugs 
*************************************************************************/
void reset();

/*************************************************************************
 *  FUNCTION execute
**************************************************************************
*   This function performs the vertical piping on multiple pipelines 
*************************************************************************/
void multiple_pipeline(char *command, char **arguments);


/*************************************************************************
 *  FUNCTION grep 
**************************************************************************
*   This function returns the absolute pathname of a word  
*************************************************************************/
char *grep(char *fileName, char *word); 
