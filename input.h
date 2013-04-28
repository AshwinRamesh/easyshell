#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
/* Command Format: <command> [args...] [<] arg1 [> | >>] arg2 &*/
struct inputStruct { // struct for the parsing of input given by users
    char * command;
    int numArgs;
    char ** args[MAX_ARGS];
    char * commandAndArgs[MAX_ARGS]; // array that includes the command and the proceeding args (no input output or background included)
    int inputRedir; // 0 -> no input ; 1 -> input given
    char * inputArg;
    int outputRedir; // 0 -> no output ; 1 -> ">" output ; 2 -> ">>" output
    char * outputArg;
    int backgroundExec; // 0 -> no background exec ; 1 -> background exec
    int correctFormat; // 0 -> not correct ; 1 -> correct
} ;

struct inputStruct * initialiseInputStruct(struct inputStruct * tempInput);
struct inputStruct * parseInput(char ** args, int numArgs);
#endif
