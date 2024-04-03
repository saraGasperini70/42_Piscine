<<<<<<< HEAD
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
=======
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
>>>>>>> 3f98121a4b69bd905c3987b1c7c2200b3cf92681
}

char	*ft_strdup(char *src)
{
<<<<<<< HEAD
	char	*dst = malloc(ft_strlen(src));

	if (!dst)
		return (NULL);
=======
	char	*dst;

	dst = malloc(sizeof(char *) * ft_strlen(src) + 1);
	if (!dst)
	{
		return (NULL);
	}
>>>>>>> 3f98121a4b69bd905c3987b1c7c2200b3cf92681
	ft_strcpy(dst, src);
	return (dst);
}

char	*ft_strjoin(char *s1, char *s2)
{
<<<<<<< HEAD
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
=======
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
>>>>>>> 3f98121a4b69bd905c3987b1c7c2200b3cf92681
}

char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE + 1];
	char		*line;
	char		*newline;
<<<<<<< HEAD
	int		to_copy;
	int		readcount;

	line = ft_strdup(buf);
	while (!(newline = ft_strchr(line, '\n')) && (readcount = read(fd, buf, BUFFER_SIZE)))
=======
	int			to_copy;
	int			readcount;

	line = ft_strdup(buf);
	while (!(newline = ft_strchr(line, '\n')) && (readcount = read(fd, buf, BUFFER_SIZE) > 0))
>>>>>>> 3f98121a4b69bd905c3987b1c7c2200b3cf92681
	{
		buf[readcount] = '\0';
		line = ft_strjoin(line, buf);
	}
	if (ft_strlen(line) == 0)
		return (free(line), NULL);
<<<<<<< HEAD
=======
	newline = ft_strchr(line, '\n');
>>>>>>> 3f98121a4b69bd905c3987b1c7c2200b3cf92681
	if (newline != NULL)
	{
		to_copy = newline - line + 1;
		ft_strcpy(buf, newline + 1);
	}
	else
	{
		to_copy = ft_strlen(line);
<<<<<<< HEAD
		buf[0] = '\0';
=======
		ft_strcpy(buf, "");
>>>>>>> 3f98121a4b69bd905c3987b1c7c2200b3cf92681
	}
	line[to_copy] = '\0';
	return (line);
}
<<<<<<< HEAD
=======

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
>>>>>>> 3f98121a4b69bd905c3987b1c7c2200b3cf92681
