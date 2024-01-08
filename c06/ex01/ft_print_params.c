/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_params.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgasperi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 10:02:59 by sgasperi          #+#    #+#             */
/*   Updated: 2022/12/08 10:03:22 by sgasperi         ###   ########.fr       */
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

	c = 1;
	if (ac)
	{
		while (av[c])
		{
			ft_putstr(av[c]);
			write (1, "\n", 1);
			c++;
		}
	}
	return (0);
}
