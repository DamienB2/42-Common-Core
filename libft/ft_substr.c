/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 10:56:18 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/14 11:58:43 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	len_s;
	size_t	i;
	
	if (!s)
		return (NULL);

	len_s = ft_strlen(s);
	if (start >= len_s)
	{
		sub = malloc(1);
		if (!sub)
			return (NULL);
		sub[0] = '\0';
		return (sub);
	}

	if (len > len_s - start)
		len = len_s - start;

	sub = malloc(sizeof(char) * (len + 1));
	if(!sub)
		return (NULL);

	i = 0;
	while (s[start + i] != '\0' && i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

/*#include <stdio.h>
int	main(void)
{
	char	*str = "Bonjour tout le monde";
	char	*res;

	res = ft_substr(str, 3, 4);
	printf("Test 1 (Normal)  : [%s]\n", res);
	free(res);

	res = ft_substr(str, 16, 50);
	printf("Test 2 (Trop long): [%s]\n", res);
	free(res);

	res = ft_substr(str, 50, 5);
	printf("Test 3 (Hors lim) : [%s]\n", res);
	free(res);

	return (0);
}*/
