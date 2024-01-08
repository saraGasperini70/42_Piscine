/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rush04.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfaramel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 17:50:16 by vfaramel          #+#    #+#             */
/*   Updated: 2022/11/19 20:37:58 by vfaramel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char c);

void	first_line(int x)

{
	int	i;

	i = 2;
	ft_putchar('/');
	while (i < x)
	{
		ft_putchar('*');
		i++;
	}
	if (x > 1)
	{
		ft_putchar('\\');
	}
	ft_putchar('\n');
}

void	middle_line(int x, int y)

{
	int	i;
	int	z;

	i = 2;
	while (i < y)
	{
		z = 2;
		ft_putchar('*');
		while (z < x)
		{
			ft_putchar(' ');
			z++;
		}
		if (x > 1)
		{
			ft_putchar('*');
		}
		ft_putchar('\n');
		i++;
	}
}

void	last_line(int x)

{
	int	i;

	i = 2;
	ft_putchar('\\');
	while (i < x)
	{
		ft_putchar('*');
		i++;
	}
	if (x > 1)
		ft_putchar('/');
	ft_putchar('\n');
}

void	rush(int x, int y)

{
	if (x <= 0 || y <= 0)
	{
		return ;
	}
	first_line(x);
	if (y > 2)
		middle_line(x, y);
	if (y > 1)
		last_line(x);
}
