/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:55:12 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/21 11:53:22 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (size <= dst_len)
		return (size + src_len);
	i = 0;
	while (src[i] && (dst_len + i) < (size - 1))
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}

/*#include <stdio.h>
int main(void)
{
    char src[] = "source";
    char dest[10] = "dest";
    int size = 0;

    printf("Avant cat:\n");
    printf("src  = %s\n", src);
    printf("dest = %s\n", dest);

    size = ft_strlcat(dest, src, 11);

    printf("\nAprès cat:\n");
    printf("src  = %s\n", src);
    printf("dest = %s\n", dest);
    printf("size = %i\n", size);

    return (0);
}*/
