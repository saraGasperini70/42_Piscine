/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rev_params.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgasperi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 10:03:57 by sgasperi          #+#    #+#             */
/*   Updated: 2022/12/08 10:04:37 by sgasperi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		write (1, &str[i], 1);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	int	c;

	c = ac - 1;
	if (ac)
	{
		while (c > 0)
		{
			ft_putstr(av[c]);
			write(1, "\n", 1);
			c--;
		}
	}
	return (0);
}
