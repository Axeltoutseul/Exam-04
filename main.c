#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int send_error(const char *str)
{
    int i = 0;
    while (str[i])
        write(2, &str[i++], 1);
    return (1);
}

int cd(int i, char **argv)
{
    if (i != 2)
        return send_error("Bad argument\n");
    if (chdir(argv[1]) == -1)
        return send_error("chdir\n");
    return 0;
}

int micro_shell(char **argv, int i, char **envp)
{
    int pipe_fd[2];
    int status = 0;
    int has_pipe = argv[i] && strcmp(argv[i], "|") == 0;

    if (has_pipe && pipe(pipe_fd) == -1)
        return (send_error("pipe"));
    int pid = fork();
    if (pid == 0)
    {
        argv[i] = 0;
        if (has_pipe && (dup2(pipe_fd[1], 1) == -1 || close(pipe_fd[0]) == -1 || close(pipe_fd[1]) == -1))
            return (send_error("dup2"));
        if (strcmp(argv[0], "cd") == 0)
            cd(i, argv);
        execve(*argv, argv, envp);
    }
    waitpid(pid, &status, 0);
    if (has_pipe && (dup2(pipe_fd[0], 0) == -1 || close(pipe_fd[0]) == -1 || close(pipe_fd[1]) == -1))
        send_error("dup2");
    return (WIFEXITED(status) && WEXITSTATUS(status));
}

int main(int argc, char **argv, char **envp)
{
    int i = 0;
    int status = 0;

    if (argc < 2)
        return -1;
    while (argv[i] && argv[++i])
    {
        argv += i;
        i = 0;
        while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
            i++;
        if (i > 0)
            status = micro_shell(argv, i, envp);
    }
    return 0;
}
