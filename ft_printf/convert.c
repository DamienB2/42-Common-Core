/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 15:53:19 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/28 15:54:21 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*convert(unsigned long num, int base, int isUpper)
{
	char		*tab;
	static char	buffer[50];
	char		*ptr;

	if (isUpper)
		tab = "0123456789ABCDEF";
	else
		tab = "0123456789abcdef";
	ptr = &buffer[49];
	*ptr = '\0';
	if (num == 0)
	{
		*--ptr = tab[0];
		return (ptr);
	}
	while (num != 0)
	{
		*--ptr = tab[num % base];
		num /= base;
	}
	return (ptr);
}
