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

// system error function
void syserr(char * msg){
   fprintf(stderr,"%s: %s", strerror(errno), msg);
   abort();
}

// set the parent of a child process to myshell
void set_child_parent(){
    setenv("parent", getenv("shell"), 1);
}

// function to escape exit signals
void exit_signals(int signal) {
    printf("\nIllegal Operation. Press \"Enter\" to continue. To exit the shell use the \"quit\" command.\n");
    return;
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
    // check for illegal formats
    if(tempInput->correctFormat == 0 || tempInput->numArgs > 1 || tempInput->inputRedir != 0 || tempInput->outputRedir != 0 || tempInput->backgroundExec ==1) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <cd> [arg1]");
        return -1;
    }
    // print current dir
    if (tempInput->numArgs == 0) {
        fprintf(stdout, "%s\n", getenv("PWD"));
        return 0;
    }
    // directory doesnt exist
    if(chdir(*tempInput->args[0]) != 0) {
        fprintf(stdout, "%s\n", "Directory does not exist");
        return -1;
    }
    // directory exists. change env PWD
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
    //check for illegal format
    if (tempInput->correctFormat == 0 || tempInput->numArgs > 0 || tempInput->inputRedir != 0 || tempInput->outputRedir != 0 || tempInput->backgroundExec == 1) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <clear>");
        return -1;
    }
    switch(pid = fork()){
        case -1:
            syserr("Error occured during executing command");
        case 0:
            set_child_parent();
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
    //check for illegal format
    if(tempInput->correctFormat == 0 || tempInput->numArgs != 1 || tempInput->inputRedir != 0 || tempInput->backgroundExec ==1 ) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <dir> <arg1> [>|>> outputArg]");
        return -1;
    }
    switch(pid = fork()){
        case -1:
            syserr("Error occured during executing command");
            return -1;
        case 0:
            set_child_parent();
            // check for type of output redirection
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
    // check for illegal format
    if(tempInput->correctFormat == 0 || tempInput->numArgs != 0 || tempInput->inputRedir != 0  || tempInput->backgroundExec == 1) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <dir> <arg1> [>|>> outputArg]");
        return -1;
    }
    int i;
    // Open output file for writing. Check for type of output redir
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
    //check for illegal format
    if(tempInput->correctFormat == 0  || tempInput->inputRedir != 0  || tempInput->backgroundExec == 1) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <echo> [arg1 ... argN] [>|>> outputArg]");
        return -1;
    }
    // no args given
    if(tempInput->numArgs == 0) { // no echo args
        switch(pid = fork()){
            case -1:
                syserr("Error occured during executing command");
            case 0:
                set_child_parent();
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
            set_child_parent();
            // check for type of output redir
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
    // check for illegal format
    if(tempInput->correctFormat == 0  || tempInput->numArgs > 0 || tempInput->inputRedir != 0  || tempInput->backgroundExec == 1) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <help>  [>|>> outputArg]");
        return -1;
    }
    // allocate memory for readme directory
    char * readme = malloc((sizeof(char) * strlen(dir)) + strlen("/readme"));
    strcpy(readme,dir);
    strcat(readme,"/readme");
    //fprintf(stdout, "%s\n", readme);
    switch(pid = fork()){
        case -1:
            syserr("Error occured during executing command");
        case 0:
            set_child_parent();
            // check for output redir type
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
    // check for illegal format
    if(tempInput->correctFormat == 0  || tempInput->numArgs > 0 || tempInput->inputRedir != 0  || tempInput->outputRedir != 0 || tempInput->backgroundExec == 1) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <help>  [>|>> outputArg]");
        return -1;
    }
    // pause the shell
    static struct termios oldt, newt; // define structs
    tcgetattr(STDIN_FILENO,&oldt); // get the attributes of current shell and place in oldt
    newt = oldt; // make newt the same as oldt
    newt.c_lflag &= ~(ICANON | ECHO); // change part of newt which controls output to false
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // set these attributes for newt

    int c = 0;
    fprintf(stdout, "%s\n", "Press ENTER to continue...");
    while ((c = getchar())) { // check for character input
        if (c == '\n' || c== EOF) { // check if it is EOF or ENTER is pressed
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // make terminal back to oldt self
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
    pid_t pid;
    int status;
    // check for illegal commands
    if (tempInput->correctFormat == 0) {
        fprintf(stdout, "%s\n", "Command cannot be parsed. Usage: <command> [arg1 arg2 ... argN] [< inputArg] [>|>> outputArg] [&]");
        return -1;
    }
    // check if the input redir is there and if the file actually exists
    if (tempInput->inputRedir == 1 && access(tempInput->inputArg,R_OK) != 0) {
        fprintf(stdout, "%s\n", "Input file cannot be accessed or does not exist.");
        return -1;
    }
    switch(pid = fork()){
        case -1:
            syserr("Error occured during executing command");
        case 0:
            set_child_parent();
            // check for type of output redir
            if (tempInput->outputRedir == 1) {
                freopen(tempInput->outputArg, "w+", stdout);
            }
            else if (tempInput->outputRedir == 2) {
                freopen(tempInput->outputArg, "a+", stdout);
            }
            // check for input redir
            if (tempInput->inputRedir == 1) { // input redirection
                freopen(tempInput->inputArg, "r", stdin);
            }
            // execute external command and check if that command is correct or not
            if (execvp(tempInput->commandAndArgs[0],tempInput->commandAndArgs) == -1) { // failure of command
                fprintf(stdout, "%s\n", "Command not found.");
                _exit(EXIT_FAILURE); // exit the fork
            }
        default:
            // check for background exec
            if ( tempInput->backgroundExec == 0) { // background exec
                waitpid(pid,&status,WUNTRACED);
            }
    }
    return 0;
}
