#include "shell.h"

/**
 * child_process - handles the child process execution
 * @full_cmd: full path of the command
 * @argv: array of arguments
 */
void child_process(char *full_cmd, char **argv)
{
	if (execve(full_cmd, argv, environ) == -1)
	{
		perror("./hsh");
		free(full_cmd);
		exit(EXIT_FAILURE);
	}
}

/**
 * execute_command - forks and executes a command
 * @argv: array of arguments
 * @last_status: pointer to last exit status
 * @line: input line (for freeing if error)
 */
void execute_command(char **argv, int *last_status, char *line)
{
	pid_t pid;
	int status;
	char *full_cmd = find_path(argv[0]);

	if (full_cmd == NULL)
	{
		fprintf(stderr, "./hsh: 1: %s: not found\n", argv[0]);
		*last_status = 127;
		return;
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
		child_process(full_cmd, argv);
	else
	{
		wait(&status);
		*last_status = WEXITSTATUS(status);
		free(full_cmd);
	}
}

/**
 * read_input - reads and processes user input
 * @line: pointer to line buffer
 * @argv: array of arguments
 *
 * Return: 1 if input processed, 0 to continue, -1 to exit
 */
int read_input(char **line, char **argv)
{
	size_t len = 0;
	ssize_t read;
	char *trimmed, *token;
	int i = 0;

	read = getline(line, &len, stdin);
	if (read == -1)
		return (-1);

	if ((*line)[read - 1] == '\n')
		(*line)[read - 1] = '\0';

	trimmed = trim_whitespace(*line);
	if (*trimmed == '\0')
		return (0);

	token = strtok(trimmed, " \t");
	while (token != NULL && i < 63)
		argv[i++] = token, token = strtok(NULL, " \t");
	argv[i] = NULL;

	return (1);
}

/**
 * main - simple shell loop
 *
 * Return: exit status
 */
int main(void)
{
	char *line = NULL;
	char *argv[64];
	int last_status = 0, builtin_result, input_status;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, ":) ", 3);

		input_status = read_input(&line, argv);
		if (input_status == -1)
		{
			free(line);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			return (last_status);
		}
		if (input_status == 0)
			continue;

		builtin_result = handle_builtins(argv);
		if (builtin_result == 1)
			break;
		if (builtin_result == 2)
			continue;

		execute_command(argv, &last_status, line);
	}
	free(line);
	return (last_status);
}
