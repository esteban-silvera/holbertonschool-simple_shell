#include <sys/types.h>
#include <stddef.h>
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#define MAX 10000

/**
 * free_tokens - The function realize a frees a arrays of tokens
 *
 * @tokens: recived one array the tokens
 **/

void free_tokens(char **tokens)
{
	int x = 0;

	while (tokens[x])
	{
	free(tokens[x]);
	x++;
	}
	x = 0;
}


/**
 * main - The main function simulates the operation of a mini shell
 *
 * Return: retun when the function is end
 **/

int main(void)
{	int ntok = 0, i = 0;
	char *prompt = "#:D ", *delim = " \n\t", *line = NULL, *tokens[MAX] = {0};
	char *tok, *aux;
	ssize_t read;
	size_t len = 0;
	struct stat check;

	while (1)
	{	memset(tokens, 0, sizeof(tokens));
	printf("%s", prompt);
	read = getline(&line, &len, stdin);
	if (read == -1)
	return (127);
	quitarsalto(line);
	tok = strtok(line, delim);
	if (tok == NULL)
	continue;
	while (tok != NULL && i < MAX)
	{
	ntok++;
	tokens[i] = strdup(tok);
	tok = strtok(NULL, delim);
	i++; }
	tokens[i] = NULL;
	i++;
	if (strcmp(tokens[0], "exit") == 0)
	{
	free_tokens(tokens);
	break; }
	if (stat(tokens[0], &check) == 0)
	{
	aux = strdup(tokens[0]);
	executor(tokens, &aux); }
	else
	{
	whicher(tokens, &aux); }
	free_tokens(tokens);
	i = 0; }
	free(line);
	return (0); }


/**
 * backtothepath - This function obtein the "PATH"
 *
 * Return: return NULL when the function is end
 **/

char *backtothepath()
{
	int i;

	for (i = 0; environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
		return (environ[i] + 5);
		}
	}
	return (NULL);
}

/**
 * executor - This function performs the command execution by receiving the
 * token as a command and receiving the address with aux
 *
 * @tokens: its a comand for execute
 *
 * @aux: its a drection go execute
 **/

void executor(char **tokens, char **aux)
{
	pid_t childpid;
	int status, i = 0;
	
	childpid = fork();
	if (childpid == 0) {
	while (tokens[i]) {
	i++;
	}
	if (execve(*aux, tokens, environ) == -1) {
	}
	} else if (childpid > 0) {
	wait(&status);	
	free(*aux);
    
	} else {
	printf("ERROR CANT FORK\n");
	exit(0);
	}
}

/**
 * whicher - This function searches the path for a command that receives
 * the token and looks for the address it is at and then
 * executes it with the address of aux.
 *
 * @tokens: This is the comand searches
 *
 * @aux: This the addres
 **/

void whicher(char **tokens, char **aux)
{
	struct stat check;
	char *path_copy, *sh;
	char *toks;
	char *tokensp[MAX] = {0};
	char *slash = "/";
	ssize_t checks = 0;
	int i = 0;

	path_copy = strdup(backtothepath());
	toks = strtok(path_copy, ":");
	while (toks != NULL)
	{
	tokensp[i] = strdup(toks);
	toks = strtok(NULL, ":");
	i++;
	}
	i = 0;
	while (tokensp[i] != NULL)
	{
	sh = malloc(strlen(tokensp[i]) + strlen(slash) + strlen(tokens[0]) + 1);
	strcpy(sh, tokensp[i]);
	strcat(sh, slash);
	strcat(sh, tokens[0]);
	checks = stat(sh, &check);
	if (checks == 0)
	{
	*aux = sh;
	break;
	}
	free(sh);
	i++;
	}
	if (checks == -1)
	{
	aux = NULL;
	printf("command error whicher\n");
	}
	free_tokens(tokensp);
	
	if (aux) 
	{
	executor(tokens, aux);
	}
	free(path_copy);
}
