#include <stdio.h>
#include <stdlib.h>

void	prime_num(char *number)
{
	int	nb;
	int	i;

	nb = atoi(number);
	i = 1;
	if (nb == 1)
		printf("1");
       	while (nb >= ++i)
	{
		if (nb % i == 0)
		{
			printf("%d", i);
			if (nb == i)
				break ;
			printf("*");
			nb /= i;
			i = 1;
		}
	}		
}

int	main(int ac, char **av)
{
	if (ac == 2)
		prime_num(av[1]);
	printf("\n");
	return (0);
}
