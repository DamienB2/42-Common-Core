/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 11:41:34 by dbaise            #+#    #+#             */
/*   Updated: 2026/05/07 13:58:23 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	process(int count, const char *pos, va_list arg)
{
	if (*pos == 'c')
		count += ft_putchar(va_arg(arg, int));
	else if (*pos == 's')
		count += ft_putstr(va_arg(arg, char *));
	else if (*pos == 'p')
		count += ft_putaddr(va_arg(arg, void *));
	else if (*pos == 'd' || *pos == 'i')
		count += ft_putnbr(va_arg(arg, int));
	else if (*pos == 'u')
		count += ft_unsigned_putnbr(va_arg(arg, unsigned int));
	else if (*pos == 'x')
		count += ft_putstr(convert(va_arg(arg, unsigned int), 16, 0));
	else if (*pos == 'X')
		count += ft_putstr(convert(va_arg(arg, unsigned int), 16, 1));
	else if (*pos == '%')
		count += ft_putchar('%');
	return (count);
}

int	ft_printf(const char *format, ...)
{
	int			count;
	const char	*pos;
	va_list		arg;

	va_start(arg, format);
	pos = format;
	count = 0;
	while (*pos)
	{
		if (*pos == '%')
		{
			pos++;
			count = process(count, pos, arg);
			if (*pos)
				pos++;
		}
		else
		{
			ft_putchar(*pos);
			pos++;
			count++;
		}
	}
	va_end(arg);
	return (count);
}
