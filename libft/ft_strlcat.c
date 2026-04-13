/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:55:12 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/13 11:00:51 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t dst_len = 0;
	size_t src_len = 0;
	size_t i = 0;

	while (dst[dst_len] && dst_len < size)
		dst_len++;

	while (src[src_len])
		src_len++;

	if (size <= dst_len)
		return (size + src_len);

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

    size = ft_strlcat(dest, src, 10);

    printf("\nAprès cat:\n");
    printf("src  = %s\n", src);
    printf("dest = %s\n", dest);
    printf("size = %i\n", size);

    return (0);
}*/
