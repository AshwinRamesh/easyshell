#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "utility.h"
#include <sys/wait.h>
#include <sys/stat.h>

extern int errno;


void syserr(char * msg){
   fprintf(stderr,"%s: %s", strerror(errno), msg);
   abort();
}

/* getPrompt - returns the full prompt message for the shell (returns char* )*/
/* Always assign this function return to a variable so that you can free the memory */
char *getPrompt(char *promptStart, char *promptEnd) {
        char * currentDir = getenv("PWD");
        int promptLen = strlen(currentDir) + strlen(promptStart) + strlen(promptEnd);
        char *prompt = malloc(sizeof(char) * promptLen);
        strcpy(prompt,promptStart);
        strcat(prompt,currentDir);
        strcat(prompt,promptEnd);
        return prompt;
}

int changeDirectory(char **directory,int numArgs){
    if (numArgs == 1) {
        fprintf(stdout, "%s\n", getenv("PWD"));
        return 0;
    }
    else if (numArgs > 2) {
        fprintf(stdout, "%s\n", "Usage: cd <Directory>");
        return 0;
    }
    if(chdir(directory[1]) != 0) {
        fprintf(stdout, "%s\n", "Directory does not exist");
        return -1;
    }
    else {
        char cwd[1024];
        getcwd(cwd,sizeof(cwd));
        setenv("PWD",cwd,1);
    }
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

int listDirectory(char **directory,int numArgs){
    pid_t pid;
    int status;
    if(numArgs != 2) {
        fprintf(stdout, "%s\n", "Usage: dir <directory-name>");
        return -1;
    }
    switch(pid = fork()){
        case -1:
            syserr("Error occured during executing command");
        case 0:
            execl("/bin/ls","ls","-la",directory[1],(char *)NULL);
        default:
            waitpid(pid,&status,WUNTRACED);
    }

    return 0;
}

int listEnvironmentVars(char **environ){
    int i;
    for (i = 0; environ[i] != NULL; i++) {
        fprintf(stdout,"%s\n",environ[i]);
    }
    return 0;
}

int echoString(char **args,int numArgs) {
    pid_t pid;
    int status;
    if(numArgs == 1) { // no echo args
        switch(pid = fork()){
            case -1:
                syserr("Error occured during executing command");
            case 0:
                execl("/bin/echo","echo",(char *)NULL);
            default:
                waitpid(pid,&status,WUNTRACED);
        }
        return 0;
    }
    switch(pid = fork()){
        case -1:
            syserr("Error occured during executing command");
        case 0:
            execv("/bin/echo",args);
        default:
            waitpid(pid,&status,WUNTRACED);
    }
    return 0;
}

int help(char * dir){
    pid_t pid;
    int status;
    char * readme = malloc((sizeof(char) * strlen(dir)) + strlen("/readme"));
    strcpy(readme,dir);
    strcat(readme,"/readme");
    switch(pid = fork()){
        case -1:
            syserr("Error occured during executing command");
        case 0:
            execl("/bin/more","more",readme,(char *)NULL);
        default:
            waitpid(pid,&status,WUNTRACED);
    }
    free(readme);
    return 0;
}

int pauseShell(int numArgs){ // fix this so that it does not show the non ENTER keys http://stackoverflow.com/posts/1798833/revisions
    if (numArgs != 1) {
        printf("%s\n", "Usage: pause");
        return -1;
    }
    int c = 0;
    fprintf(stdout, "%s\n", "Press ENTER to continue...");
    while ((c = getchar())) {
        if (c == '\n' || c== EOF) {
            return 0;
        }
    }
    return 0;
}

int externalCommand() {
    return 0;
}
