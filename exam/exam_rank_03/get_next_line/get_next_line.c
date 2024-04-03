#include <unistd.h>
#include <stdlib.h>

size_t	ft_strlen(char *str)
{
	size_t	i = 0;

	while (str[i])
	{
		i++;
	}
	return (i);
}

char	*ft_strchr(char *str, char c)
{

	while (*str)
	{
		if (*str == c)
			return ((char *)str);
		str++;
	}
	return (NULL);
}

void	ft_strcpy(char *dst, char *src)
{
	int	i = 0;

	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

char	*ft_strdup(char *src)
{
	char	*dst = malloc(ft_strlen(src));

	if (!dst)
		return (NULL);
	ft_strcpy(dst, src);
	return (dst);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	len1 = ft_strlen(s1);
	size_t	len2 = ft_strlen(s2);
	char	*join = malloc(len1 + len2 + 1);

	if (!s1 || !s2)
		return (NULL);
	if (!join)
		return (NULL);
	ft_strcpy(join, s1);
	ft_strcpy((join + len1), s2);
	return (join);
}

char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE + 1];
	char		*line;
	char		*newline;
	int		to_copy;
	int		readcount;

	line = ft_strdup(buf);
	while (!(newline = ft_strchr(line, '\n')) && (readcount = read(fd, buf, BUFFER_SIZE)))
	{
		buf[readcount] = '\0';
		line = ft_strjoin(line, buf);
	}
	if (ft_strlen(line) == 0)
		return (free(line), NULL);
	if (newline != NULL)
	{
		to_copy = newline - line + 1;
		ft_strcpy(buf, newline + 1);
	}
	else
	{
		to_copy = ft_strlen(line);
		buf[0] = '\0';
	}
	line[to_copy] = '\0';
	return (line);
}
