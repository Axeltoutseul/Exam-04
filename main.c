#include "micro_shell.h"

int count_cmds(char **argv)
{
	int i = 0;
	int count = 0;
	if (!argv)
		return (-1);
	if (strcmp(argv[0], "|") != 0 && strcmp(argv[0], ";") != 0)
	{
		i++;
		count++;
	}
	while (argv[i])
	{
		if ((strcmp(argv[i], "|") == 0 || strcmp(argv[i], ";") == 0)
			&& argv[i + 1] && strcmp(argv[i + 1], "|") != 0 && strcmp(argv[i + 1], ";") != 0)
			count++;
		i++;
	}
	return count;
}

char ***get_cmds(char **argv)
{
	int i = 0;
	int j = 1;
	int count;
	int size = count_cmds(argv);
	char ***cmds = (char ***)malloc(sizeof(char **) * (size + 1));
	if (!cmds)
		return (NULL);
	while (i < size)
	{
		count = count_lines(argv + j);
		cmds[i] = get_command(count, argv + j);
		j += count;
		while (argv[j] && (strcmp(argv[j], "|") == 0 || strcmp(argv[j], ";") == 0))
			j++;
		i++;
	}
	cmds[i] = 0;
	return (cmds);
}

void free_2d_array(char **strs)
{
	int i = 0;
	while (strs[i])
		free(strs[i++]);
	free(strs);
}

void free_cmds(char ***cmds)
{
	int i = 0;
	while (cmds[i])
		free_2d_array(cmds[i++]);
	free(cmds);
}

int main(int argc, char **argv, char **envp)
{
	(void)envp;
	int pipe_fd[2];
	pipe_fd[0] = 0;
	pipe_fd[1] = 1;
	if (argc < 2)
		return -1;
	char ***cmds = get_cmds(argv);
	microshell(cmds[0], envp, pipe_fd);
	free_cmds(cmds);
	return 0;
}
