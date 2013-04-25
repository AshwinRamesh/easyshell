#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "input.h"

/*
    Input.c defines and parses the input into the shell
*/

struct inputStruct * initialiseInputStruct(struct inputStruct * tempInput) {
    tempInput->numArgs = 0;
    tempInput->inputRedir = 0;
    tempInput->outputRedir = 0;
    tempInput->backgroundExec = 0;
    tempInput->correctFormat = 1;
    return tempInput;
}

// Parse input
struct inputStruct * parseInput(char ** args, int numArgs) {
    struct inputStruct *tempInput = malloc(sizeof(struct inputStruct) *1);
    initialiseInputStruct(tempInput);
    int i;
    tempInput->command = args[0];
    tempInput->commandAndArgs[0] = &args[0];
    for (i=1; i < numArgs; i++) {
        if (!strcmp(args[i],">")){
                if (i == numArgs - 1) { // last argument to be read. means that format is wrong
                    tempInput->correctFormat = 0;
                    return tempInput;
                }
                if (tempInput->outputRedir != 0) { // more than one output redir
                    tempInput->correctFormat = 0;
                    return tempInput;
                }
                tempInput->outputRedir = 1;
                tempInput->outputArg = args[i+1];
                i = i + 1; // increment since we have read the next argument as the outputArg
        }
        else if (!strcmp(args[i],">>")){
                if (i == numArgs - 1) { // last argument to be read. means that format is wrong
                    tempInput->correctFormat = 0;
                    return tempInput;
                }
                if (tempInput->outputRedir != 0) { // more than one output redir
                    tempInput->correctFormat = 0;
                    return tempInput;
                }
                tempInput->outputRedir = 2;
                tempInput->outputArg = args[i+1];
                i = i + 1; // increment since we have read the next argument as the outputArg
        }
        else if (!strcmp(args[i],"<")){
                if (i == numArgs - 1) { // last argument to be read. means that format is wrong
                    tempInput->correctFormat = 0;
                    return tempInput;
                }
                if (tempInput->inputRedir != 0) { // more than one input redir
                    tempInput->correctFormat = 0;
                    return tempInput;
                }
                if(tempInput->outputRedir != 0) { // output redirection has been defined before input redir. This is not supported
                    tempInput->correctFormat = 0;
                    return tempInput;
                }
                tempInput->inputRedir = 1;
                tempInput->inputArg = args[i+1];
                i = i + 1;
        }
        else if (!strcmp(args[i],"&")){
                if (i != numArgs-1) { // background exec arg has to be last arg
                    tempInput->correctFormat = 0;
                    return tempInput;
                }
                tempInput->backgroundExec = 1;
        }
        else {
                if((tempInput->inputRedir != 0) || (tempInput->outputRedir != 0) || (tempInput->backgroundExec != 0)) { // these args have to be defined before all I/O and background exec args
                    tempInput->correctFormat = 0;
                    return tempInput;
                }
                tempInput->args[tempInput->numArgs] = &args[i];
                tempInput->commandAndArgs[tempInput->numArgs + 1] = &args[i]; // increment to consolodate for command being array[0] element
                tempInput->numArgs = tempInput->numArgs + 1;
        }
    }
    return tempInput;
}
