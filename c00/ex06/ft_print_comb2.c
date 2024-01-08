/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_comb2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgasperi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 11:37:13 by sgasperi          #+#    #+#             */
/*   Updated: 2022/12/05 09:30:05 by sgasperi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char c)
{
	write (1, &c, 1);
}

void	ft_pcomb(int n1, int n2)
{
	char	str1;
	char	str2;

	str1 = (n1 - (n1 % 10)) / 10 + '0';
	str2 = n1 % 10 + '0';
	ft_putchar(str1);
	ft_putchar(str2);
	ft_putchar(' ');
	str1 = (n2 - (n2 % 10)) / 10 + '0';
	str2 = n2 % 10 + '0';
	ft_putchar(str1);
	ft_putchar(str2);
	if (!(n1 == 98 && n2 == 99))
		write (1, ", ", 2);
}

void	ft_print_comb2(void)
{
	int	n1;
	int	n2;

	n1 = 0;
	n2 = 0;
	while (1)
	{
		while (++n2 <= 99)
		{
			ft_pcomb(n1, n2);
		}
		n1++;
		n2 = n1;
		if (n1 == 99)
		{
			write (1, "\n", 1);
			return ;
		}
	}
}
