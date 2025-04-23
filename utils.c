#include "shell.h"

/**
 * trim_whitespace - removes leading and trailing whitespaces
 * @str: input string
 *
 * Return: trimmed string
 */
char *trim_whitespace(char *str)
{
	char *end;

	while (*str == ' ' || *str == '\t' || *str == '\n')
		str++;

	if (*str == '\0')
		return (str);

	end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
		end--;

	*(end + 1) = '\0';
	return (str);
}

/**
 * get_path_env - gets PATH environment variable
 *
 * Return: pointer to PATH string
 */
char *get_path_env(void)
{
	int i = 0;

	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5);
		i++;
	}
	return (NULL);
}

/**
 * find_path - searches for a command in PATH
 * @command: command to find
 *
 * Return: full path or NULL
 */
char *find_path(char *command)
{
	char *path = get_path_env();
	char full_path[1024];
	char *token, *ptr;
	int len;

	if (!command)
		return (NULL);

	if (strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (strdup(command));
		else
			return (NULL);
	}

	if (!path || path[0] == '\0')
		return (NULL);

	ptr = path;
	while (*ptr)
	{
		token = ptr;
		while (*ptr && *ptr != ':')
			ptr++;

		len = ptr - token;
		snprintf(full_path, sizeof(full_path), "%.*s/%s", len, token, command);

		if (access(full_path, X_OK) == 0)
			return (strdup(full_path));

		if (!*ptr)
			break;
		ptr++;
	}
	return (NULL);
}

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
