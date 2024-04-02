#include <unistd.h>

int	ft_strlen(char *str)
{
	int	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	check(int c, char *str, int index)
{
	int	i = 0;

	while (i < index)
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	int	i;
	int	j;
	int	k;

	if (ac == 3)
	{
		i = ft_strlen(av[1]);
		j = 0;
		while (av[2][j])
		{
			av[1][i] = av[2][j];
			i++;
			j++;
		}
		i--;
		k = 0;
		while (k < i)
		{
			if (check(av[1][k], av[1], k) == 0)
				write (1, &av[1][k], 1);
			k++;
		}
	}
	write (1, "\n", 1);
	return (0);
}
