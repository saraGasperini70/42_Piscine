#include "get_next_line.h"
#include <stdio.h>

char	*ft_strchr(char *str, int c)
{
	int	i = 0;
	while (str[i])
	{
		if (str[i] == (char)c)
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

char	*ft_strcpy(char *dst, char *src)
{
	int	i = 0;

	while(src[i] && dst[i])
	{
			dst[i] = src[i];
			i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_strdup(char *src)
{
	char	*dst;

	dst = malloc(sizeof(char *) * ft_strlen(src) + 1);
	if (!dst)
	{
		return (NULL);
	}
	ft_strcpy(dst, src);
	return (dst);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*result;
	int		len1;
	int		len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc((len1 + len2 + 1));
	if (!s1 || !s2 || !result)
		return (NULL);
	ft_strcpy(result, s1);
	ft_strcpy((result + len1), s2);
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
	while (!(newline = ft_strchr(line, '\n')) && (readcount = read(fd, buf, BUFFER_SIZE) > 0))
	{
		buf[readcount] = '\0';
		line = ft_strjoin(line, buf);
	}
	if (ft_strlen(line) == 0)
		return (free(line), NULL);
	newline = ft_strchr(line, '\n');
	if (newline != NULL)
	{
		to_copy = newline - line + 1;
		ft_strcpy(buf, newline + 1);
	}
	else
	{
		to_copy = ft_strlen(line);
		ft_strcpy(buf, "");
	}
	line[to_copy] = '\0';
	return (line);
}

int	main(int ac, char **av)
{
	char	*line;
	int		fd;
	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		printf("%d\n", fd);
		line = get_next_line(fd);
		while (line)
		{
			printf("%s", line);
			free(line);
			line = NULL;
		}
	}

}
