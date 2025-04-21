#include "shell.h"

extern char **environ;

int main(void)
{
	char *line = NULL, *trimmed_line, *token;
	char *argv[64];
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	int i;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

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

		trimmed_line = trim_whitespace(line);
		if (trimmed_line[0] == '\0')
			continue;

		i = 0;
		token = strtok(trimmed_line, " \t");
		while (token != NULL && i < 63)
		{
			argv[i++] = token;
			token = strtok(NULL, " \t");
		}
		argv[i] = NULL;

		if (argv[0] == NULL)
			continue;

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free(line);
			exit(EXIT_FAILURE);
		}

		if (pid == 0)
		{
			if (execve(argv[0], argv, environ) == -1)
			{
				perror("./hsh");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			wait(NULL);
		}
	}
	free(line);
	return (0);
}
