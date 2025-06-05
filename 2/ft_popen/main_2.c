#include "ft_popen.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int		ft_popen(const char *file, char *const argv[], char type);

char	*get_next_line(int fd)
{
	static char	buf[1024];
	int			i;
	char		c;

	i = 0;
	while (read(fd, &c, 1) > 0)
	{
		buf[i++] = c;
		if (c == '\n')
			break ;
	}
	buf[i] = '\0';
	return ((i > 0) ? buf : NULL);
}

void	ft_putstr(char *str)
{
	while (*str)
		write(1, str++, 1);
}

int	main(void)
{
	int		fd;
	char	*line;

	fd = ft_popen("ls", (char *const[]){"ls", "-l", NULL}, 'r');
	printf("____%d\n", fd);
	if (fd == -1)
	{
		perror("ft_popen");
		return (1);
	}
	while ((line = get_next_line(fd)))
		ft_putstr(line);
	close(fd);
	return (0);
}
