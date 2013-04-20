#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "myshell.h"

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

/* keep reading input until "quit" command or eof of redirected input */
    while (!feof(stdin)) {

        int promptLen = strlen(currentDir) + strlen(promptStart) + strlen(promptEnd);
        char *prompt = malloc(sizeof(char) * promptLen);
        strcpy(prompt,promptStart);
        strcat(prompt,currentDir);
        strcat(prompt,promptEnd);
/* get command line from input */

        fputs (prompt, stdout);                // write prompt
        free(prompt);
        if (fgets (buf, MAX_BUFFER, stdin )) { // read a line

/* tokenize the input into args array */

            arg = args;
            *arg++ = strtok(buf,SEPARATORS);   // tokenize input
            while ((*arg++ = strtok(NULL,SEPARATORS)));
                                               // last entry will be NULL

            if (args[0]) {                     // if there's anything there

/* check for internal/external command */

                if (!strcmp(args[0],"clear")) { // "clear" command
                    system("clear");
                    continue;
                }

                if (!strcmp(args[0],"quit"))   // "quit" command
                    break;                     // break out of 'while' loop

/* else pass command onto OS (or in this instance, print them out) */

                arg = args;
                while (*arg) fprintf(stdout,"%s ",*arg++);
                fputs ("\n", stdout);
            }
        }
    }
    return 0;
}
