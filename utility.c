#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "utility.h"

/* getPrompt - returns the full prompt message for the shell (returns char* )*/
/* Always assign this function return to a variable so that you can free the memory */
char *getPrompt(char *promptStart, char *promptEnd, char *currentDir) {
        int promptLen = strlen(currentDir) + strlen(promptStart) + strlen(promptEnd);
        char *prompt = malloc(sizeof(char) * promptLen);
        strcpy(prompt,promptStart);
        strcat(prompt,currentDir);
        strcat(prompt,promptEnd);
        return prompt;
}

int changeDirectory(){
	return 0;
}

int clearScreen(){
	return 0;
}

int listDirectory(){
	return 0;
}

int listEnvironmentVars(){
	return 0;
}

int echo() {
	return 0;
}

int help(){
	return 0;
}

int pause(){
	return 0;
}

int quit(){
	return 0;
}

int externalCommand() {
	return 0;
}
