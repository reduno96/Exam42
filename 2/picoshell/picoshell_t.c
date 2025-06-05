#include "picoshell.h"

int picoshell(char **cmds[])
{
	int i = 0;
	int res = 0;
	int pre_fd = -1;
	int st;
	int pid;
	int fd[2];
	   int ret = 0;

	while(cmds[i])
	{
		if (cmds[i+1])
		   pipe(fd);
		pid = fork();
		if (pid == -1)
			return 1;
		if (pid == 0)
		{
			if (pre_fd != -1)
			{
				dup2(pre_fd, 0);
				close(pre_fd);
			}
			if (cmds[i + 1])
			{
				dup2(fd[1], 1);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (pre_fd != -1)
			close(pre_fd);
		if (cmds[i + 1])
		{
			close(fd[1]);
			pre_fd = fd[0];
		}
		i++;
	}
	
    while (wait(&st) != -1)
    {
    
       
    }

    return ret;
}
