#include <unistd.h>
void	ft_putstr_n(char str, int n)
{
	while (n > 0)
	{	
		write (1, &str, 1);
		--n;
	}
}

void	repeat_alpha(char *str)
{
	while (*str != '\0')
	{
		if (*str >= 'a' && *str <= 'z')
			ft_putstr_n(*str, *str + 1 - 'a');
		else if (*str >= 'A' && *str <= 'Z')
			ft_putstr_n(*str, *str + 1 - 'A');
		else
			write (1, str, 1);
		str++;
	}
}

int	main(int ac , char **av)
{
	if (ac == 2)
		repeat_alpha(av[1]);
	write (1, "\n", 1);
	return (0);
}
