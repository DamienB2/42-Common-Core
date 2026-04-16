/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:26:41 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/16 17:26:42 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_putnbr_fd(int n, int fd)
{
    long    nb;
    char    c;

    nb = n;
    if(nb < 0)
    {
        write(fd, "-", 1);
        nb = -nb;
    }
    if(nb >= 10)
        ft_putnbr_fd(nb / 10, fd);
    c = (nb % 10) + '0';
    write(fd, &c, 1);
}

