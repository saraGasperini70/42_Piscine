#include <stdio.h>

int	max(int *tab, unsigned int len)
{
	int	ret = 0;
	unsigned int	i = 0;

	while (tab[i] && i < len)
	{
		if (tab[i] > ret)
			ret = tab[i];
		i++;
	}
	return (ret);
}

