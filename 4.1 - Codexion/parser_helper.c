/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 17:25:57 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/11 11:39:05 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_is_str_digit(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int				i;
	long long int	result;

	i = 0;
	result = 0;
	if (nptr[i] == '+')
	{
		i++;
	}
	if (nptr[i] == '\0')
		return (-1);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		if (result > 2147483647)
			return (-1);
		i++;
	}
	return ((int)(result));
}
