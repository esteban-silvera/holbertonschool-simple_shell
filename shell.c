#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#define MAX 10000

void free_tokens(char **tokens)
{
	int x = 0;

	while(tokens[x])
	{
	free(tokens[x]);
	x++;
	}

	x = 0;
}

void quitarsalto(char *line)
{
	int len = strlen(line);
	if (len > 0 && line[len - 1] == '\n')
	{
	line[len - 1] = '\0';
	}
}

int main(void)
{
	int j = 0, ntok = 0, o = 1, i = 0, checker = 0;
	char *ppath, *tok, *line = NULL;
	char *prompt = "#:D ", *aux;
	ssize_t read;
	size_t len = 0;
	char *delim = " \n\t";
	struct stat check;
	char *tokens[MAX] = {0};
	char *path = "PATH";

	while (1)
	{
	memset(tokens, 0, sizeof(tokens));
	printf("%s", prompt);
	read = getline(&line, &len, stdin);
	if (read == -1) {
	printf("ERROR\n");
	break;
	}
	quitarsalto(line);
	tok = strtok(line, delim);
	if (tok == NULL) {
	continue;
	}

	while (tok != NULL && i < MAX) {
	ntok++;
	tokens[i] = strdup(tok);
	tok = strtok(NULL, delim);
	i++;
	}
	tokens[i] = NULL;
	i++;
	if (strcmp(tokens[0], "exit")==0)
	{
	free_tokens(tokens);
	break;
	}
	checker = stat(tokens[0], &check);
	if (checker == 0) {
	executor(tokens, &aux);
	} else {
	whicher(tokens, &aux);
	}
	free_tokens(tokens);
	i = 0;
	}
	free(line);
	return 0;
}

char *backtothepath()
{
	extern char **environ;
	for (int i = 0; environ[i] != NULL; i++)
	{
	if (strncmp(environ[i], "PATH=", 5) == 0)
	{
	return environ[i] + 5;
	}
	}
	return NULL;
}

void executor(char **tokens, char **aux)
{
	pid_t childpid;
	int status;
	childpid = fork();
	if (childpid == 0) {
	execv(*aux, tokens);
	} else if (childpid > 0) {
	wait(&status);
	} else {
	printf("ERROR CANT FORK\n");
	exit(0);
	}
}

void whicher(char **tokens, char **aux)
{
	struct stat check;
	char *path_copy, *sh;
	char *toks;
	char *tokensp[MAX] = {0};
	char *slash = "/";
	ssize_t checks = 0;
	int i = 0, o = 1, j = 0;

	j = 0;
	path_copy = strdup(backtothepath());
	toks = strtok(path_copy, ":");
	while (toks != NULL) {
	tokensp[i] = strdup(toks);
	toks = strtok(NULL, ":");
	i++;
	}
	i = 0;
	while (tokensp[i] != NULL) {
	sh = malloc(strlen(tokensp[i]) + strlen(slash) + strlen(tokens[0]) + 1);
	strcpy(sh, tokensp[i]);
	strcat(sh, slash);
	strcat(sh, tokens[0]);
	checks = stat(sh, &check);
	if (checks == 0) {
	*aux = sh;
	break;
	}
	free(sh);
	i++;
	}
	if (checks == -1) {
	printf("command error whicher\n");
	}
	free_tokens(tokensp);
	executor(tokens, aux);
	free(sh);
	free(path_copy);
}
