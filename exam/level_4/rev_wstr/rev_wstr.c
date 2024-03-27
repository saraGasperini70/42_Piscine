#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int	ft_strlen(char *str)
{
	int	i = 0;

	while (str[i])
		i++;
	return (i);
}

void	ft_putchar(char c)
{
	write (1, &c, 1);
}

void	rev_wstr(char *str)
{
	int	len = ft_strlen(str);
	char	*rev = NULL;

	len--;	
	while(str[len])
	{
		if (str[len - 1] == ' ')
		{	rev = &str[len];
			while (*rev && *rev != 32)
			{
				ft_putchar(*rev),
				rev++;
			}
			ft_putchar(' ');		
		}
		else if (len == 0)
		{
			rev = &str[len];
			while (*rev && *rev != 32)
			{
				ft_putchar(*rev);
				rev++;
			}
		}
		len--;
	}
	
}

int	main(int ac, char **av)
{
	if (ac == 2)
		rev_wstr(av[1]);
	ft_putchar('\n');
}
