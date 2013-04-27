#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "input.h"
#include "utility.h"
#include <termios.h>
#include <signal.h>

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token sparators

extern char **environ;

int main (int argc, char ** argv) {
    char buf[MAX_BUFFER];                       // line buffer
    char * args[MAX_ARGS];                      // pointers to arg strings
    char ** arg;                                // working pointer thru args
    char * promptStart = "simpleShell~(";
    char * promptEnd = ") ==> " ;                     // shell prompt
    char * shellName = "myshell";
    char * startingDir = getenv("PWD");
    int numArgs;
    /* Set SHELL Env to myshell */
    char shellPath[MAX_BUFFER];
    getcwd(shellPath,MAX_BUFFER);
    strcat(shellPath,"/");
    strcat(shellPath,shellName);
    setenv("shell",shellPath,1);

    if (argc > 2) {
        fprintf(stdout, "%s\n", "Too many arguments. Syntax: myshell [batchfile]");
        return -1; // too many args
    }
    else if (argc == 2) {
        if (access(argv[1],R_OK) == 0) { // check if the file exists
            freopen(argv[1], "r", stdin);
        }
        else {
            fprintf(stdout, "%s\n", "File cannot be opened or does not exist.");
            return -1;
        }
    }
    /* Read input until "quit" */
    while (!feof(stdin)) {
        /* Write prompt to command line */
        if (argc == 1) { // if there is a batch file then don't print the prompt
            char *prompt = getPrompt(promptStart,promptEnd);
            fputs (prompt, stdout);                 // write prompt
            free(prompt);                           // free memory that was allocated through the getPrompt function
        }

        if (signal(SIGINT, exit_signals));// Escape all signals

        if (fgets (buf, MAX_BUFFER, stdin )) {  // read a line

            /* tokenize the input into args array */
            arg = args;
            *arg++ = strtok(buf,SEPARATORS);   // tokenize input

            numArgs = 1; // take into account for the first argument
            while ((*arg++ = strtok(NULL,SEPARATORS))){
                numArgs++;
            }

            if (args[0]) {                     // if there's anything there
                struct inputStruct * userInput;
                userInput = parseInput(args,numArgs);
                //fprintf(stdout, "Command: %s Correct Format: %d Input: %d Output: %d Background: %d\n", userInput->command,userInput->correctFormat, userInput->inputRedir, userInput->outputRedir, userInput->backgroundExec);

/* check for internal/external command */
                if (!strcmp(userInput->command,"clear")) { // "clear" command
                    clearScreen(userInput);
                    continue;
                }
                else if (!strcmp(userInput->command,"environ")) { // "clear" command
                    listEnvironmentVars(environ,userInput);
                    continue;
                }
                else if(!strcmp(userInput->command,"cd")) { // "cd" command
                    changeDirectory(userInput);
                    continue;
                }
                else if(!strcmp(userInput->command,"dir")) { // "dir" command
                    listDirectory(userInput);
                    continue;
                }
                else if(!strcmp(userInput->command,"help")) { // "help" command
                    help(startingDir,userInput);
                    continue;
                }
                else if(!strcmp(userInput->command,"echo")) { // "echo" command
                    echoString(userInput);
                    continue;
                }
                else if(!strcmp(userInput->command,"pause")) { // "pause" command
                    pauseShell(userInput);
                    continue;
                }
                else if (!strcmp(userInput->command,"quit")){ // "quit" command
                    if (userInput->correctFormat == 0  || userInput->numArgs > 0 || userInput->inputRedir != 0  || userInput->outputRedir != 0 || userInput->backgroundExec == 1) {
                        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <quit>");
                        continue;
                    }
                    else {
                        break;
                    }
                }
                else { // other commands
                    externalCommand(userInput);
                    //arg = args;
                    //while (*arg) fprintf(stdout,"%s ",*arg++);
                    //fputs ("\n", stdout);
                }
                free(userInput);
            }
        }
    }
    return 0;
}
