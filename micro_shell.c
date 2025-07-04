#include "micro_shell.h"

void exec_child_proccess(char **cmd, int pipe_fd[2], char **envp)
{
    close(pipe_fd[1]);
    if (dup2(pipe_fd[0], 1) == -1)
        return (perror("dup2"));
    close(pipe_fd[0]);
    execve(cmd[0], &cmd[0], envp);
    // free memory
    perror("execve");
    exit(1);
}

void microshell(char **cmd1, char **envp, int pipe_fd[2])
{
    (void)cmd1;
    int status;
    pid_t pid;
    if (pipe(&pipe_fd[2]) == -1)
        perror("pipe");
    else
    {
        pid = fork();
        if (pid == -1)
            perror("fork");
        else if (pid == 0)
            exec_child_proccess(cmd1, pipe_fd, envp);
        waitpid(pid, &status, 0);
        // exec_parent_proccess(cmd2, pipe_fd, envp);
    }
}
