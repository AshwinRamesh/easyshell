#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "utility.h"
#include <sys/wait.h>

extern int errno;


void syserr(char * msg){
   fprintf(stderr,"%s: %s", strerror(errno), msg);
   abort();
}

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
	pid_t pid;
	int status;
	switch(pid = fork()){
		case -1:
			syserr("Error occured during executing command");
		case 0:
			execl("/usr/bin/clear","clear",(char *)NULL);
		default:
			waitpid(pid,&status,WUNTRACED);
	}
	return 0;
}

int listDirectory(){
	return 0;
}

int listEnvironmentVars(char **environ){
	int i;
	for (i = 0; environ[i] != NULL; i++) {
		fprintf(stdout,"%s\n",environ[i]);
	}
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

int externalCommand() {
	return 0;
}
