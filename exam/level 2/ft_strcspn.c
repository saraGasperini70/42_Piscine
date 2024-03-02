#include <string.h>
#include <stdio.h>

size_t	ft_strcspn(const char *s, const char *reject)
{
	int		i;
	size_t	result;

	result = 0;
	while (s[result])
	{
		i = 0;
		while (reject[i])
		{
			if (s[result] == reject[i])
				return (result);
			i++;;
		}
		result++;
	}
	return (result);
}