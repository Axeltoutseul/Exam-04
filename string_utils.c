#include "micro_shell.h"

void ft_putstr(const char *str)
{
    int i = 0;
    while (str[i])
        write(1, &str[i++], 1);
}

int ft_strlen(const char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return i;
}

char *ft_strdup(const char *src)
{
    int i = 0;
    int size = ft_strlen(src);
    char *dest = (char *)malloc(sizeof(char) * size + 1);
    if (!dest)
        return (NULL);
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
    return (dest);
}

char **get_command(int size, char **command)
{
    int i = 0;
    char **strs = (char **)malloc(sizeof(char *) * (size + 1));
    if (!strs)
        return (NULL);
    while (i < size)
    {
        strs[i] = ft_strdup(command[i]);
        i++;
    }
    strs[i] = 0;
    return (strs);
}

int count_lines(char **lines)
{
    int i = 0;
    while (lines[i] && strcmp(lines[i], "|") != 0 && strcmp(lines[i], ";") != 0)
        i++;
    return i;
}
