#include <unistd.h>
#include <string.h>
#include <stdio.h>

size_t	ft_strspn(const char *s, const char *accept)
{
	int		i;
	int		j;
	size_t	result;

	i = 0;
	result = 0;
	while (s[i] != '\0')
	{
		j = 0;
		while (accept[j] != '\0')
		{
			if (s[i] != accept[j])
				break ;
			j++;
		}
		i++;
	}
	return (i);
}

int	main(void)
{
	printf("strspn: %lu\n", strspn("hello world", "hell"));
	printf("ft_strspn: %lu\n", ft_strspn("hello world", "hell"));
	return (0);
}