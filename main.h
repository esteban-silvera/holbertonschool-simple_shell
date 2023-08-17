#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
int main (void);
void executor(char **tokens, char **aux);
void whicher(char **tokens, char **aux);
void quitarsalto(char *line);
char *backtothepath();
void free_tokens(char **tokens);
