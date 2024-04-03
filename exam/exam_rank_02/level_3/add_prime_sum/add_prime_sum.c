#include <unistd.h>
#include <stdio.h>

void ft_print_nbr(int nbr)
{
	char *str = "0123456789";
	if (nbr > 9)
		ft_print_nbr(nbr / 10);
	write (1, &str[nbr % 10], 1);
}

int	ft_atoi(char *nbr)
{
	int	res = 0;
	int	i = 0;
	int 	sign = 1;
	
	if (nbr[i] <= 32)
		i++;
	if (nbr[i] == '+' || nbr[i] == '-')
	{
		if (nbr[0] == '-')
			sign *= -1;
		i++;
	}
	while (nbr[i] >= '0' && nbr[i] <= '9')
	{
		res += res * 10 + nbr[i] - '0';
		i++;
	}
	return (res);
}

int	is_prime(int nbr)
{
	int	div = 2;

	while (div <= nbr / 2)
	{
		if (nbr % div == 0)
			return (0);
		div++;
	}
	return (1);
}

int	main(int ac, char **av)
{
	int	result = 0;
	int	n = 0;

	if (ac != 2 || ft_atoi(av[1]) <= 0)
		write (1, "0", 1);
	else if (ac == 2)
	{
		n = ft_atoi(av[1]);
		while (n > 1)
		{
			if (is_prime(n) == 1)
				result += n;
			n--;
		}
		ft_print_nbr(result);
	}
	write (1, "\n", 1);
	return (0);
}
