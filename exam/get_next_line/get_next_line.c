#include "get_next_line.h"

char	*ft_strchr(char *str, char c)
{
	int	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return ((char *)str);
		i++;
	}
	return (NULL);
}

int	ft_strlen(char *str)
{
	int	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strlcpy(char *dst, char *src, int dstlen)
{
	int	i = 0;
	int	srclen = ft_strlen(src);

	if (dstlen > 0)
	{
		while(i < srclen && i < dstlen)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (dst);
}

char	*ft_strdup(char *src)
{
	char	*dst;

	dst = malloc(sizeof(char *) * ft_strlen(src));
	if (!dst)
		return (NULL);
	dst = ft_strlcpy(dst, src, ft_strlen(src));
	return (dst);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*result;
	int	i = 0;
	int	j = 0;
	result = malloc(sizeof(char *) * (ft_strlen(s1) + ft_strlen(s2)));

	while (s1[i])
	{
		result[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i])
	{
		result[j] = s2[i];
		i++;
		j++;
	}
	return (result);
}

