#include <unistd.h>

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write (1, &str[i], 1);
		i++;
	}
}

int	main(int ac, char **av)
{
	int	i = 0;
	int	j = 0;

	if (ac == 3)
	{
		while (av[2][j])
		{
			if (av[2][j] == av[1][i])
				i++;
			j++;
		}
		if (!av[1][i])
			ft_putstr(av[1]);
	}
	write (1, "\n", 1);
	return (0);
}
