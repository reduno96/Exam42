#include "ft_popen.h"

int	ft_popen(const char *file, char *const argv[], char type)
{
	pid_t	pid;
	int		fd[2];

	if (!file || !argv || (type != 'w' && type != 'r'))
		return (-1);
	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		if (type == 'r')
		{
			if (close(fd[0]) == -1 || dup2(fd[1], STDOUT_FILENO) == -1
				|| close(fd[1]) == -1)
				return (-1);
		}
		if (type == 'w')
		{
			if (close(fd[1]) == -1 || dup2(fd[0], STDIN_FILENO) == -1
				|| close(fd[0]) == -1)
				return (-1);
		}
		execvp(file, argv);
		_exit(EXIT_FAILURE);
	}
	if (type == 'r')
	{
		close(fd[1]);
		return (fd[0]);
	}
	else if (type == 'w')
	{
		close(fd[0]);
		return (fd[1]);
	}
	return (-1);
}
