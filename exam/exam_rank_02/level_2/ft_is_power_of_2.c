#include <stdlib.h>
#include <stdio.h>

int	    is_power_of_2(unsigned int n)
{
	if (n == 0)
		return (0);
	if (n % 2 == 0)
		n /= 2;
	printf("n = %d\n", n);
	if (n == 1)
		return (1);
	else
		return (0);
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		if (is_power_of_2(atoi(av[1])))
			printf("%s is power of 2\n", av[1]);
		else
			printf("%s is not power of 2\n", av[1]);
	}
	return (0);
}