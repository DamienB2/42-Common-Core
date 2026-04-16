/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 16:06:20 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/16 16:06:22 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void    ft_striteri(char *s, void (*f)(unsigned int, char*))
{
    unsigned int    i;

    if(!s || !f)
        return ;
    i = 0;
    while(s[i])
    {
        f(i, &s[i]);
        i++;
    }
}

/*void my_test_func(unsigned int i, char *c)
{
    if (*c >= 'a' && *c <= 'z')
        *c -= 32;
}

#include <stdio.h>
int main(void)
{
    char str[] = "hello world";

    printf("Avant : %s\n", str);

    ft_striteri(str, my_test_func);

    printf("Après : %s\n", str);

    return (0);
}*/