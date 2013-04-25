#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED
extern int errno;
void syserr(char *msg);
char *getPrompt(char* promptStart, char* promptEnd);
int changeDirectory(char ** directory,int numArgs);
int clearScreen(struct inputStruct * tempInput);
int listDirectory(char **directory,int numArgs);
int listEnvironmentVars(char **environ);
int echoString(char **args,int numArgs) ;
int pauseShell(int numArgs);
int help(char * dir);
int quitShell();

#endif
