#include <unistd.h>
int	ft_atoi(char *num)
{
	int	i;

	i = 0;
	while (*num != '\0')
	{
		i = i * 10;
		i = i + *num - '0';
		++num;
	}
	return (i);
}

void	print_hex(int num)
{
	char 	*base;
	
	base = "0123456789abcdef";
	if (num > 16)
		print_hex(num / 16);
	write (1, &base[num % 16], 1);
}

int	main(int ac, char **av)
{
	if (ac == 2)
		print_hex(ft_atoi(av[1]));
	write (1, "\n", 1);
}
