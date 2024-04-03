#include <unistd.h>
#include <stdio.h>

int	ft_atoi(char *str)
{
	int	res = 0;
	int	sign = 1;
	int	i = 0;

	if (str[i] <= 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * sign);
}

void	ft_print_nbr(int nbr)
{
	char *str = "0123456789";

	if (nbr > 9)
		ft_print_nbr(nbr / 10);
	write (1, &str[nbr % 10], 1);
}

void	is_prime(int nbr)
{
	int	div = 2;

	while (div != nbr)
	{
		if (nbr % div == 0)
		{
			printf("%i*", div);
			nbr /= div;
			div = 1;
		}
		div++;
	}
	printf("%i", div);
}

int	main(int ac, char **av)
{
	int	n;      
	
	
	if (ac == 2)
	{
		n = ft_atoi(av[1]);
		if (n > 0)
		{
			if (n == 1)
				printf("1\n");
			is_prime(n);
		}
	}
	printf("\n");
}
