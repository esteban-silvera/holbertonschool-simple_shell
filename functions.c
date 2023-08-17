#include "main.h"

/**
 * quitarsalto - The function recived a line and removes the final line break
 *
 * @line: The line
 **/

void quitarsalto(char *line)
{
	int len = strlen(line);

	if (len > 0 && line[len - 1] == '\n')
	{
	line[len - 1] = '\0';
	}
}

