#include <unistd.h>
#include <stdio.h>

int	main(int ac, char **av)
{
	int	i = 0;
	int	j;
	if (ac == 2)
	{
		while (av[1][i] && ((av[1][i] == ' ') || (av[1][i] == '\t')))
			i++;
		j = i;
		while (av[1][i])
		{
			while (av[1][i] && ((av[1][i] != ' ') && (av[1][i] != '\t')))
				i++;
			while (av[1][i] && ((av[1][i] == ' ') || (av[1][i] == '\t')))
				i++;
			while (av[1][i] && ((av[1][i] != ' ') && (av[1][i] != '\t')) && ((av[1][i - 1] == ' ') || (av[1][i - 1] == '\t')))
			{
				while (av[1][i] && ((av[1][i] != ' ') && (av[1][i] != '\t')))
				{
					write (1, &av[1][i], 1);
					i++;
				}
				write (1, " ", 1);
				i++;
			}
		}
		while (av[1][j] > 32)
		{
			write (1, &av[1][j], 1);
			j++;
		}
	}
	write (1, "\n", 1);
	return (0);
}
