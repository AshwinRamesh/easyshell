#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED
extern int errno;
void syserr(char *msg);
char *getPrompt(char* promptStart, char* promptEnd);
int changeDirectory(char ** directory,int numArgs);
int clearScreen();
int listDirectory(char **directory,int numArgs);
int listEnvironmentVars(char **environ);
int echo(char **args,int numArgs) ;
int help(char * dir);
int quitShell();

#endif
