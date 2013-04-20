#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "utility.h"

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token sparators

extern char **environ;

int main (int argc, char ** argv) {
    char buf[MAX_BUFFER];                       // line buffer
    char * args[MAX_ARGS];                      // pointers to arg strings
    char ** arg;                                // working pointer thru args
    char * currentDir = getenv("PWD");                     // current directory
    char * promptStart = "simpleShell~(";
    char * promptEnd = ") ==> " ;                     // shell prompt
    char * shellName = "myshell";

    /* Set SHELL Env to myshell */
    char shellPath[MAX_BUFFER];
    getcwd(shellPath,MAX_BUFFER);
    strcat(shellPath,"/");
    strcat(shellPath,shellName);
    setenv("SHELL",shellPath,1);

    /* Read input until "quit" */
    while (!feof(stdin)) {
        /* Write prompt to command line */
        char *prompt = getPrompt(promptStart,promptEnd,currentDir);
        fputs (prompt, stdout);                 // write prompt
        free(prompt);                           // free memory that was allocated through the getPrompt function

        if (fgets (buf, MAX_BUFFER, stdin )) {  // read a line

            /* tokenize the input into args array */
            arg = args;
            *arg++ = strtok(buf,SEPARATORS);   // tokenize input
            while ((*arg++ = strtok(NULL,SEPARATORS)));
                                               // last entry will be NULL

            if (args[0]) {                     // if there's anything there

/* check for internal/external command */

                if (!strcmp(args[0],"clear")) { // "clear" command
                    clearScreen();
                    continue;
                }

                if (!strcmp(args[0],"environ")) { // "clear" command
                    listEnvironmentVars(environ);
                    continue;
                }

                if (!strcmp(args[0],"quit")){ // "quit" command
                    break;
                }

/* else pass command onto OS (or in this instance, print them out) */

                arg = args;
                while (*arg) fprintf(stdout,"%s ",*arg++);
                fputs ("\n", stdout);
            }
        }
    }
    return 0;
}
