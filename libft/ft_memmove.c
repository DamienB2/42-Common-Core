/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 14:22:52 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/10 15:22:50 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t	i;

	d = (unsigned char *) dest;
	s = (unsigned char *) src;
	i = 0;

	if (!dest || !src)
		return(0);

	if (d < s)
	{
		while (i < n)
		{
                	d[i] = s[i];
                	i++;
		}
	}
	else if (d > s)
	{
		i = n;
		while (i > 0)
		{
			i--;
			d[i] = s[i];
		}
	}
	return (dest);	
}

/*#include <stdio.h>
int main(void)
{
	char str[]	= "123456";
	char str1[]	= "123456";

	memmove(str + 1, str, 3);
	ft_memmove(str1 + 1, str1, 3);

	printf("memmove  : %s\n", str);
	printf("ft_memmove : %s\n", str1);
	return (0);
}*/
