#include <unistd.h>

void	ft_putchar(char c)
{
	write (1, &c, 1);
}

void	alpha_mirror(char *str)
{
	int	i = 0;

	while (str[i] != '\0')
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = 'M' - (str[i] - 'N');
		else if (str[i] >= 'a' && str[i] <= 'z')
			str[i] = 'm' - (str[i] - 'n');
		ft_putchar(str[i]);
		i++;
	}
}

int	main(int ac, char **av)
{
	if (ac == 2)
		alpha_mirror(av[1]);
	write (1, "\n", 1);
	return (0);
}
