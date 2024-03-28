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

size_t	ft_strlcpy(char *dst, char *src, int dstlen)
{
	int	i = 0;
	int	srclen = ft_strlen(src);

	if (dstlen > 0)
	{
		while(i < srclen && i < dstlen - 1)
		{
			dst[i] = src[i];
			write (1, &dst[i], 1);
			i++;
		}
		dst[i] = '\0';
	}
	return (srclen);
}

char	*ft_strdup(char *src)
{
	char	*dst;

	dst = malloc(sizeof(char *) * ft_strlen(src));
	if (!dst)
	{
		return (NULL);
	}
	ft_strlcpy(dst, src, ft_strlen(src));
	return (dst);
}

char	*ft_strjoin(char *s1, char *s2, size_t len)
{
	char	*result;
	int		len1;
	int		len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = len;
	result = malloc(sizeof(char *) * (len1 + len2));
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1, len1 + 1);
	ft_strlcpy(result + len1, s2, len2 + 1);
	free(s1);
	return (result);
}

char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE + 1];
	char		*line;
	char		*newline;
	int			to_copy;
	int			readcount;

	line = ft_strdup(buf);
	while (!(ft_strchr(line, '\n')) && (readcount = read(fd, buf, BUFFER_SIZE) > 0))
	{
		buf[readcount] = '\0';
		line = ft_strjoin(line, buf, readcount);
	}
	if (ft_strlen(line) == 0)
		return (free(line), NULL);
	newline = ft_strchr(line, '\n');
	if (newline != NULL)
	{
		to_copy = newline - line + 1;
		ft_strlcpy(buf, newline + 1, BUFFER_SIZE + 1);
	}
	else
	{
		to_copy = ft_strlen(line);
		ft_strlcpy(buf, "", BUFFER_SIZE + 1);
	}
	line[to_copy] = '\0';
	return (line);
}
