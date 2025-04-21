#include "shell.h"

extern char **environ;

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	char *argv[2], *command;

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

		command = trim_whitespace(line);
		if (command[0] == '\0')
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
			argv[0] = command;
			argv[1] = NULL;
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
