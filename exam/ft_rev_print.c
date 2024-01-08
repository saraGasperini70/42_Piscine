/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rev_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgasperi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 11:04:24 by sgasperi          #+#    #+#             */
/*   Updated: 2022/12/08 11:29:10 by sgasperi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char	*ft_rev_print(char *str)
{
	int	len;

	len = ft_strlen(str);
	len--;
	while (len >= 0)
	{
		write (1, &str[len], 1);
		len--;
	}
	return (str);
}

int	main()
{
	char c[] = "Hello World";
	ft_rev_print(c);
}
