#ifndef MICRO_SHELL_H
#define MICRO_SHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int count_lines(char **lines);
void ft_putstr(const char *str);
char *ft_strdup(const char *src);
int ft_strlen(const char *str);
char **get_command(int size, char **command);
void microshell(char **cmd1, char **envp, int pipe_fd[2]);

#endif
