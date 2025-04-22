#include "shell.h"

int main(void)
{
	char *line = NULL, *trimmed, *token, *full_cmd;
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	char *argv[64];
	int i;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, ":) ", 3);

		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			free(line);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			return (0);
		}

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		trimmed = trim_whitespace(line);
		if (*trimmed == '\0')
			continue;

		i = 0;
		token = strtok(trimmed, " \t");
		while (token != NULL && i < 63)
		{
			argv[i++] = token;
			token = strtok(NULL, " \t");
		}
		argv[i] = NULL;

		full_cmd = find_path(argv[0]);
		if (full_cmd == NULL)
		{
			perror("./hsh");
			continue;
		}

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free(line);
			free(full_cmd);
			exit(EXIT_FAILURE);
		}

		if (pid == 0)
		{
			if (execve(full_cmd, argv, environ) == -1)
			{
				perror("./hsh");
				free(full_cmd);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			wait(NULL);
			free(full_cmd);
		}
	}
	free(line);
	return (0);
}
