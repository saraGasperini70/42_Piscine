#include <stdio.h>
int	is_valid(char c, int str_base)
{
	char str3[17] = "0123456789abcdef";
	char str2[17] = "0123456789ABCDEF";
	while (str_base--)
		if (c == str3[str_base] || c == str2[str_base])
			return (1);
	return (0);
}

int	ft_isblank(char c)
{
	if (c <= 32)
		return (1);
	return (0);
}

int	ft_value(char s)
{
	if (s >= '0' && s <= '9')
		return (s - '0');
	else if (s >= 'A' && s <= 'F')
		return (s - 'A' + 10);
	else if (s >= 'a' && s <= 'f')
		return (s - 'a' + 10);
	return (0);
}

int	ft_atoi_base(const char *str, int str_base)
{
	int	result = 0;
	int	sign = 1;
	int	i = 0;
	
	while (ft_isblank(str[i]))
		i++;
	sign = (*str == '-') ? -1 : 1;
	(*str == '-' || *str == '+') ? ++str : 0;
	while (is_valid(*str, str_base))
		result = result * str_base + ft_value(*str++);
	return (result * sign);
}
