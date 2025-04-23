#include "shell.h"

/**
 * handle_builtins - handles built-in commands (exit, env)
 * @argv: array of arguments
 *
 * Return: 1 for exit, 2 for env, 0 otherwise
 */
int handle_builtins(char **argv)
{
	int i;

	if (argv[0] == NULL)
		return (0);

	if (strcmp(argv[0], "exit") == 0)
		return (1);

	if (strcmp(argv[0], "env") == 0)
	{
		for (i = 0; environ[i] != NULL; i++)
		{
			write(STDOUT_FILENO, environ[i], strlen(environ[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		return (2);
	}

	return (0);
}
