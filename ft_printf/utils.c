/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 11:21:05 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/28 14:05:38 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putstr(char *s)
{
	if (!s)
		return (write(1, "(null)", 6));
	return (write(1, s, ft_strlen(s)));
}

int	ft_putnbr(int n)
{
	int		count;
	char	c;

	count = 0;
	if (n == -2147483648)
		return (write(1, "-2147483648", 11));
	if (n < 0)
	{
		count += write(1, "-", 1);
		n = -n;
	}
	if (n >= 10)
		count += ft_putnbr(n / 10);

	c = (n % 10) + '0';
	count += write(1, &c, 1);
	return (count);
}

int	ft_unsigned_putnbr(unsigned int n)
{
	int		count;
	char	c;

	count = 0;
	if (n >= 10)
		count += ft_putnbr(n / 10);

	c = (n % 10) + '0';
	count += write(1, &c, 1);
	return (count);
}
char	*convert(unsigned int num, int base, int isUpper)
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
