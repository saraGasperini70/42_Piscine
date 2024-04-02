
#include <stdlib.h>


int	ft_strlen(char *str)
{
	int	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strrev(char *str)
{
	int	i = 0;
	int	len = ft_strlen(str);
	char	tmp;

	while (len > i)
	{
		len--;
		tmp = str[i];
		str[i] = str[len];
		str[len] = tmp;
		i++;		
	}
	return (str);

}

char	*ft_itoa(int nbr)
{
	int	i = 0;
	int	sign = 0;
	char	*str = malloc(sizeof(char) * 12);

	if (str == NULL)
		return (NULL);
	if (nbr == 0)
		return ("0");
	if (nbr == -2147483648)
		return ("-2147483648");
	if (nbr < 0)
	{
		sign = 1;
		nbr *= -1;
	}
	while (nbr)	
	{
		str[i++] = (nbr % 10) + '0';
		nbr /= 10;
	}
	if (sign == 1)
		str[i] = '-';
	return ft_strrev(str);
}

