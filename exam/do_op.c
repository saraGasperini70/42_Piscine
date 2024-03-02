#include <unistd.h>
#include <stdlib.h>

void	ft_print_number(int nb)
{
	if (nb > 9)
		ft_print_number(nb / 10);
	write(1, &"0123456789"[nb % 10], 1);
}

int	main(int ac, char **av)
{
	int	s1;
	int	s2;
	int result;
	
	result = 0;
	if (ac == 4)
	{
		s1 = atoi(av[1]);
		s2 = atoi(av[3]);
		if (av[2][0] == '+')
			result = s1 + s2;
		else if (av[2][0] == '-')
			result = s1 - s2;
		else if (av[2][0] == '*')
			result = s1 * s2;
		else if (av[2][0] == '/')
			result = s1 / s2;
		else if (av[2][0] == '%')
			result = s1 % s2;
		ft_print_number(result);
	}
	write (1, "\n", 1);
}