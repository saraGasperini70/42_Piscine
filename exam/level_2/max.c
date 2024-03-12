int		max(int* tab, unsigned int len)
{
	if (len == 0)
		return (0);
	unsigned int	i;
	int	result;
	i = 0;
	result = tab[i];
	while (i < len)
	{
		if (tab[i] > result)
			result = tab[i];
		i++;
	}
	return (result);
}
