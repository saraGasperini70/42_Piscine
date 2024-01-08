/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rev_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgasperi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 12:27:09 by sgasperi          #+#    #+#             */
/*   Updated: 2022/12/01 12:06:28 by sgasperi         ###   ########.fr       */
/*                                                                            */
/* ****************************************************************************/
#include <unistd.h>

int	ft_strlen(char *str)
{
	int i;
	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

char *ft_rev_print (char *str)
{
	int i;
	i = ft_strlen(str);
	i--;
	while (i >= 0)
	{
		write (1, &str[i], 1);
		i--;
	}
	return (str);
}

int	main()
{
	char str[] = "hello, world";
	ft_rev_print(str);
}



