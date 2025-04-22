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

char *get_path_env(void)
{
	int i = 0;

	while (environ[i])
	{
		if (environ[i][0] == 'P' && environ[i][1] == 'A' &&
		    environ[i][2] == 'T' && environ[i][3] == 'H' &&
		    environ[i][4] == '=')
		{
			return (environ[i] + 5);
		}
		i++;
	}
	return (NULL);
}

char *find_path(char *command)
{
	char *path = get_path_env();
	char full_path[1024];
	char *token, *ptr;
	int len;

	if (!command)
		return (NULL);

	if (access(command, X_OK) == 0)
		return (strdup(command));

	if (!path)
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
