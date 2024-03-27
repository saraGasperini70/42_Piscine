#include <unistd.h>
#include <stdio.h>

int	ft_wordlen(char *str)
{
	int	i = 0;

	while (str[i] && str[i] != ' ' && str[i] != '\t')
		i++;
	return (i);
}

int	skip_whitespace(char *s, int i)
{
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	return (i);
}

void	epur_str(char *str)
{
	int	i = 0;
	int	first_word = 1;
	int	word_len;

	i = skip_whitespace(str, i);
	while (str[i] != '\0')
	{
		if (first_word == 0)
			write (1, " ", 1);
	       	word_len = ft_wordlen(str + i);	
		write (1, str + i, word_len);
		i = i + word_len;
		first_word = 0;
		i = skip_whitespace(str, i);
	}
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		epur_str(av[1]);
	}
	write (1, "\n", 1);
	return (0);
}
