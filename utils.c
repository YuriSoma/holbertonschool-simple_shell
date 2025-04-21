#include "shell.h"

char *trim_whitespace(char *str)
{
	char *end;

	while (*str == ' ' || *str == '\t' || *str == '\n')
		str++;

	if (*str == '\0')
		return str;

	end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
		end--;

	*(end + 1) = '\0';
	return str;
}

char *find_path(char *command)
{
	char *path = getenv("PATH");
	char *path_copy, *token, *full_path;
	size_t len;

	if (command == NULL || path == NULL)
		return NULL;

	if (access(command, X_OK) == 0)
		return strdup(command);

	path_copy = strdup(path);
	token = strtok(path_copy, ":");

	while (token != NULL)
	{
		len = strlen(token) + strlen(command) + 2;
		full_path = malloc(len);
		if (full_path == NULL)
		{
			free(path_copy);
			return NULL;
		}

		snprintf(full_path, len, "%s/%s", token, command);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return full_path;
		}

		free(full_path);
		token = strtok(NULL, ":");
	}

	free(path_copy);
	return NULL;
}
