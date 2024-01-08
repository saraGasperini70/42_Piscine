#include <unistd.h>

char	ft_rotone(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		if ((str[i] >= 'a' && str[i] <= 'y') || (str[i] >= 'A' && str[i] <= 'Y'))
		{
			str[i] += 1;
		}
		else if (str[i] == 'z' || str[i] == 'Z')
		{
			str[i] -= 25;
		}
		write (1, &str[i], 1);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	if (ac == 2)
		ft_rotone(av[1]);
	write (1, "\n", 1);
}
