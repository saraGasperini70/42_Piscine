/*strjoin, strdup*/
#include "get_next_line.h"

char	ft_strchr(char *str, char c)
{
	int	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (str[i]);
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
