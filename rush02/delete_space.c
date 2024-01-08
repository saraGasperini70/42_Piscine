int	*delete_space(int *str)
{
	int i;
	int j;
	int swap;

	i = 0;
	j = 0;
	while (str[i] && (!((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
		i++;
	while (str[i])
	{
		swap = str[i];
		str[i] = str[j];
		str[j] = swap;
		i++;
		j++;
	}
	str[j] = '\0';
	return (*str)
}
