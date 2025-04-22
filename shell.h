#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

char *trim_whitespace(char *str);
char *find_path(char *command);
char *get_path_env(void);

#endif
