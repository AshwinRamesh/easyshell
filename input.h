#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

/* Command Format: <command> [args...] [<] arg1 [> | >>] arg2 &*/
struct inputStruct {
    char * command;
    int numArgs;
    char ** args[64];
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
