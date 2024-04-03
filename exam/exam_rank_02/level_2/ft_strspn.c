#include <unistd.h>
#include <string.h>
#include <stdio.h>

char	*ft_strchr(const char *s, char c)
{
	while (*s != '\0')
	{
		if (*s == c)
			return (char *)s;
		++s;
	}
	return (NULL);
}

size_t	ft_strspn(const char *s, const char *accept)
{
	size_t	result;

	result = 0;
	while (s[result] != '\0')
	{
		if (ft_strchr(accept, s[result]) == 0)
			break ;
		result++;
	}
	return (result);
}