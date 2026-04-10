/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 14:03:10 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/10 15:16:16 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void *ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t	i;

	d = (unsigned char *) dest;
	s = (unsigned char *) src;
	i = 0;
	
	if (!dest && !src)
		return (0);

	while (i < n)
	{
		d[i] = s[i];
		i++;
	}

	return (dest);
}

#include <stdio.h>
int main(void)
{
    char src[] = "Hello World!";
    char d1[15] = {0};
    char d2[15] = {0};

    memcpy(d1, src, 7);

    ft_memcpy(d2, src, 7);

    printf("memcpy : [%s]\n", d1);
    printf("ft_memcpy : [%s]\n", d2);
}
