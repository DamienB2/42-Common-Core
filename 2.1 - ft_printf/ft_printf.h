/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 11:45:37 by dbaise            #+#    #+#             */
/*   Updated: 2026/05/07 13:56:38 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

//ft_printf.c
int		ft_printf(const char *format, ...);

//utils.c
int		ft_putchar(char c);
int		ft_putstr(char *s);
int		ft_putnbr(int n);
int		ft_unsigned_putnbr(unsigned int n);
int		ft_putaddr(void *s);

//convert.c
char	*convert(unsigned long num, int base, int isUpper);

#endif
