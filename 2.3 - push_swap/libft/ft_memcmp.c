/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:21:53 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/22 15:05:30 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	str1 = (unsigned char *) s1;
	str2 = (unsigned char *) s2;
	if (n == 0)
		return (0);
	while (i < n)
	{
		if (str1[i] == str2[i])
			i++;
		else
			return (str1[i] - str2[i]);
	}
	return (0);
}

/*#include <stdio.h>
int     main(void)
{
        char *str1 = "Hello";
        char *str2 = "Heo";
        char *str3 = "Hello World";

        printf("Test 1 (Identiques) : %d\n", ft_memcmp(str1, str2, 2));

        printf("Test 2 (Différents) : %d\n", ft_memcmp(str1, str2, 3));

        printf("Test 3 (S1 court)   : %d\n", ft_memcmp(str1, str3, 7));

        return (0);
}*/
