/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:36:19 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/20 10:39:17 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size)
	{
		while (i < size - 1)
		{
			dst[i] = src[i];
			if (src[i] == '\0')
				return (i);
			i++;
		}
		dst[i] = '\0';
	}
	while (src[i] != '\0')
		i++;
	return (i);
}

/*#include <stdio.h>
int main(void)
{
    char src[] = "source";
    char dest[10];
    int size = 0;

    printf("Avant copie:\n");
    printf("src  = %s\n", src);
    printf("dest = %s\n", dest);

    size = ft_strlcpy(dest, src, 13);

    printf("\nAprès copie:\n");
    printf("src  = %s\n", src);
    printf("dest = %s\n", dest);
    printf("size = %i\n", size);

    return (0);
}*/
