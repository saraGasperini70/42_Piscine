#include <unistd.h>

int	last_word_len(char *str)
{
	int	i;

	while (str[i])
		i++;
	while (str[i] != '\n'&& str[i] != ' ' && str[i] != '\t')
		i--;
	return (i);
}

int	main(int ac, char **av)
{
	int	i;

	if (ac == 2)
	{
		i = last_word_len(av[1]);
		while (av[1][i])
		{
			write (1, &av[1][i], 1);
			i++;
		}
	}
	write (1, "\n", 1);
	return (0);
}
