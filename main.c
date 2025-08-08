#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void send_error(const char *str)
{
    while (*str)
        write(2, &*str++, 1);
}

void cd(int i, char **argv)
{
    if (i != 2)
        return send_error("error: cd: bad arguments\n");
    if (chdir(argv[1]) == -1)
        return send_error("error: cd: cannot change directory to "), send_error(argv[1]), send_error("\n");
}

void microshell(int i, char **argv, char **envp)
{
    pid_t pid;
    int fds[2];
    int status = 0;
    int has_pipe = argv[i] && strcmp(argv[i], "|") == 0;
    if (!has_pipe && strcmp(argv[0], "cd") == 0)
        return cd(i, argv);
    if (has_pipe && pipe(fds) == -1)
    {
        send_error("error: fatal\n");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == 0)
    {
        argv[i] = NULL;
        if (strcmp(argv[0], "cd") == 0)
            return cd(i, argv);
        if (has_pipe && (dup2(fds[1], 1) == -1 || close(fds[0]) == -1 || close(fds[1]) == -1))
        {
            send_error("error: fatal\n");
        	exit(EXIT_FAILURE);
        }
        execve(*argv, argv, envp);
        send_error("error: cannot execute "), send_error(argv[0]), send_error("\n");
    }
    waitpid(pid, &status, 0);
    if (has_pipe && (dup2(fds[0], 0) == -1 || close(fds[0]) == -1 || close(fds[1]) == -1))
    {
        return send_error("error: fatal\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv, char **envp)
{
    int i = 0;
    if (argc < 2)
        return -1;
    while (argv[i] && i < argc - 1)
    {
        ++i;
        argv += i;
        i = 0;
        while (argv[i] && strcmp(argv[i], "|") != 0 && strcmp(argv[i], ";") != 0)
            i++;
        if (i > 0)
            microshell(i, argv, envp);
    }
    return 0;
}
