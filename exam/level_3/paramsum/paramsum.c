#include <unistd.h>

void	write_number(int i)
{
	if (i > 9)
		write_number(i / 10);
	write(1, &"0123456789"[i % 10], 1);
}

int	ft_matrlen(char **av)
{
	int	i;
	
	i = 0;
	while (av[i])
		i++;
	return (i - 1);
}

int	main(int ac, char **av)
{
	int	i;

	i = ft_matrlen(av);
	if (ac >= 2)
		write_number(i);
	else
		write_number(0);
	write (1, "\n", 1);
}
