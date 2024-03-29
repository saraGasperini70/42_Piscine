#include <unistd.h>

int	iter(char *str1, char c, int len)
{
	int	i;

	i = 0;
	while (str1[i] && (i < len || len == -1))
		if (str1[i++] == c)
			return (1);
	return (0);			
}

int	main(int ac, char **av)
{
	int	i;

	if (ac == 3)
	{
		i = 0;
		while (av[1][i])
		{
			if (!iter(av[1], av[1][i], i) && iter(av[2], av[1][i], -1))
				write (1, &av[1][i], 1);
			i++;
		}
				
	}
	write (1, "\n", 1);
	return (0);
}
