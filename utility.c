#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "input.h"
#include "utility.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <termios.h>

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

/*
    cd command:
        - 0/1 arguments
        - No Input Redir
        - No Output Redir
        - No Backgroud Exec
*/
int changeDirectory(struct inputStruct * tempInput){
    if(tempInput->correctFormat == 0 || tempInput->numArgs > 1 || tempInput->inputRedir != 0 || tempInput->outputRedir != 0 || tempInput->backgroundExec ==1) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <cd> [arg1]");
        return -1;
    }
    if (tempInput->numArgs == 0) {
        fprintf(stdout, "%s\n", getenv("PWD"));
        return 0;
    }
    if(chdir(*tempInput->args[0]) != 0) {
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

/*
    clear command:
        - No arguments
        - No Input Redir
        - No Output Redir
        - No Backgroud Exec
*/
int clearScreen(struct inputStruct * tempInput){
    pid_t pid;
    int status;
    if (tempInput->correctFormat == 0 || tempInput->numArgs > 0 || tempInput->inputRedir != 0 || tempInput->outputRedir != 0 || tempInput->backgroundExec == 1) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <clear>");
        return -1;
    }
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

/*
    dir command:
        -  1 args
        - No Input Redir
        - Yes Output Redir
        - No Backgroud Exec
*/
int listDirectory(struct inputStruct * tempInput){
    pid_t pid;
    int status;
    if(tempInput->correctFormat == 0 || tempInput->numArgs != 1 || tempInput->inputRedir != 0 || tempInput->backgroundExec ==1 ) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <dir> <arg1> [>|>> outputArg]");
        return -1;
    }
    switch(pid = fork()){
        case -1:
            syserr("Error occured during executing command");
            return -1;
        case 0:
            if (tempInput->outputRedir == 1) {
                freopen(tempInput->outputArg, "w+", stdout);
            }
            else if (tempInput->outputRedir == 2) {
                freopen(tempInput->outputArg, "a+", stdout);
            }
            execl("/bin/ls","ls","-la",*tempInput->args[0],(char *)NULL);
        default:
            waitpid(pid,&status,WUNTRACED);
    }
    return 0;
}

/*
    environ command:
        - No args
        - No Input Redir
        - Yes Output Redir
        - No Backgroud Exec
*/
int listEnvironmentVars(char **environ,struct inputStruct * tempInput){
    FILE * outputFile;
    if(tempInput->correctFormat == 0 || tempInput->numArgs != 0 || tempInput->inputRedir != 0  || tempInput->backgroundExec == 1) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <dir> <arg1> [>|>> outputArg]");
        return -1;
    }
    int i;
    // Open output file for writing
    if (tempInput->outputRedir == 1) {
        outputFile = fopen(tempInput->outputArg,"w+");
    }
    else if (tempInput->outputRedir == 2) {
        outputFile = fopen(tempInput->outputArg,"a+");
    }
    // Iterate through environ vars
    for (i = 0; environ[i] != NULL; i++) {
        if (tempInput->outputRedir != 0) {
            fprintf(outputFile,"%s\n",environ[i]);
        }
        else {
            fprintf(stdout, "%s\n", environ[i]);
        }
    }
    // Close output file if required
    if (tempInput->outputRedir != 0) {
        fclose(outputFile);
    }
    return 0;
}

/*
    environ command:
        - N args
        - No Input Redir
        - Yes Output Redir
        - No Backgroud Exec
*/
int echoString(struct inputStruct * tempInput) {
    pid_t pid;
    int status;
    if(tempInput->correctFormat == 0  || tempInput->inputRedir != 0  || tempInput->backgroundExec == 1) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <echo> [arg1 ... argN] [>|>> outputArg]");
        return -1;
    }
    if(tempInput->numArgs == 0) { // no echo args
        switch(pid = fork()){
            case -1:
                syserr("Error occured during executing command");
            case 0:
                if (tempInput->outputRedir == 1) {
                    freopen(tempInput->outputArg, "w+", stdout);
                }
                else if (tempInput->outputRedir == 2) {
                    freopen(tempInput->outputArg, "a+", stdout);
                }
                execl("/bin/echo","echo",(char *)NULL);
            default:
                waitpid(pid,&status,WUNTRACED);
        }
        return 0;
    }
    // Args given
    switch(pid = fork()){
        case -1:
            syserr("Error occured during executing command");
        case 0:
            if (tempInput->outputRedir == 1) {
                freopen(tempInput->outputArg, "w+", stdout);
            }
            else if (tempInput->outputRedir == 2) {
                freopen(tempInput->outputArg, "a+", stdout);
            }
            execv("/bin/echo",tempInput->commandAndArgs);
        default:
            waitpid(pid,&status,WUNTRACED);
    }
    return 0;
}

/*
    help command:
        - No args
        - No Input Redir
        - Yes Output Redir
        - No Backgroud Exec
*/
int help(char * dir,struct inputStruct * tempInput){
    pid_t pid;
    int status;
    if(tempInput->correctFormat == 0  || tempInput->numArgs > 0 || tempInput->inputRedir != 0  || tempInput->backgroundExec == 1) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <help>  [>|>> outputArg]");
        return -1;
    }
    char * readme = malloc((sizeof(char) * strlen(dir)) + strlen("/readme"));
    strcpy(readme,dir);
    strcat(readme,"/readme");
    switch(pid = fork()){
        case -1:
            syserr("Error occured during executing command");
        case 0:
            if (tempInput->outputRedir == 1) {
                freopen(tempInput->outputArg, "w+", stdout);
            }
            else if (tempInput->outputRedir == 2) {
                freopen(tempInput->outputArg, "a+", stdout);
            }
            execl("/bin/more","more",readme,(char *)NULL);
        default:
            waitpid(pid,&status,WUNTRACED);
    }
    free(readme);
    return 0;
}

/*
    pause command:
        - No args
        - No Input Redir
        - No Output Redir
        - No Backgroud Exec
*/
int pauseShell(struct inputStruct * tempInput){ // fix this so that it does not show the non ENTER keys http://stackoverflow.com/posts/1798833/revisions
    if(tempInput->correctFormat == 0  || tempInput->numArgs > 0 || tempInput->inputRedir != 0  || tempInput->outputRedir != 0 || tempInput->backgroundExec == 1) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <help>  [>|>> outputArg]");
        return -1;
    }
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO,&oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int c = 0;
    fprintf(stdout, "%s\n", "Press ENTER to continue...");
    while ((c = getchar())) {
        if (c == '\n' || c== EOF) {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return 0;
        }
    }
    return 0;
}

/*
    external command:
        - YES args
        - YES Input Redir
        - YES Output Redir
        - YES Backgroud Exec
*/
int externalCommand(struct inputStruct * tempInput) {
    if (tempInput->correctFormat == 0) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <command> [arg1 arg2 ... argN] [< inputArg] [>|>> outputArg] [&]");
        return -1;
    }
   fprintf(stdout, "%s\n", "This external command function has not been written yet");
    return 0;
}
