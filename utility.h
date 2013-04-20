#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED
extern int errno;
void syserr(char *msg);
char *getPrompt(char* promptStart, char* promptEnd);
int changeDirectory(char ** directory,int numArgs);
int clearScreen();
int listEnvironmentVars(char **environ);
int quitShell();

#endif
