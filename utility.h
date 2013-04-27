#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED
extern int errno;
void set_child_parent();
void syserr(char *msg);
char *getPrompt(char* promptStart, char* promptEnd);
int changeDirectory(struct inputStruct * tempInput);
int clearScreen(struct inputStruct * tempInput);
int listDirectory(struct inputStruct * tempInput);
int listEnvironmentVars(char **environ,struct inputStruct * tempInput);
int echoString(struct inputStruct * tempInput) ;
int pauseShell(struct inputStruct * tempInput);
int help(char * dir,struct inputStruct * tempInput);
int externalCommand(struct inputStruct * tempInput);
#endif
