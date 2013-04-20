#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "myshell.h"

/* getPrompt - returns the full prompt message for the shell (returns char* )*/
/* Always assign this function return to a variable so that you can free the memory */
char *getPrompt(char *promptStart, char *promptEnd, char *currentDir)) {
        int promptLen = sizeof(currentDir) + sizeof(promptStart) + sizeof(promptEnd);
        char *prompt = malloc(sizeof(char) * promptLen);
        strcpy(prompt,promptStart);
        strcat(prompt,currentDir);
        strcat(prompt,promptEnd);
        return prompt;
}
