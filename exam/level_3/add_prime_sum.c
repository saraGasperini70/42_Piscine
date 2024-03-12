int	ft_atoi(char c)
{
	int	res;

	res = 0;
	if (c > 9)
		ft_atoi(c / 10);
	res = c % 10 + '0';
	return (res);
}

int	check_sign(char *c)
{
	if  (c[0] != '-')
		return (1);
	return (0);
}

int	main(int ac, char **av)
{
	int	c = 0;
	if (ac == 2)
	{
		if (check_sign(av[1]))
		{
			c = ft_atoi(av[1]);
		}
	}
}
