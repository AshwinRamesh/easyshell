#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED
struct input;
struct input * initialiseInputStruct(struct input * tempInput);
struct input * parseInput(char ** args, int numArgs);
#endif
