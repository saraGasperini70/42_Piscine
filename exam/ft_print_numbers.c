/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_numbers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgasperi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 11:44:49 by sgasperi          #+#    #+#             */
/*   Updated: 2022/12/07 11:51:16 by sgasperi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_print_numbers(void)
{
	char	i;

	i = 48;
	while(i <= 57 )
	{
		write(1, &i, 1);
		i++;
	}
}

int	main()
{
	ft_print_numbers();
}