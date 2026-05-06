/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:12:26 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/22 15:02:06 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (*little == '\0')
		return ((char *) big);
	while (big[i] != '\0' && i < len)
	{
		j = 0;
		while (big[i + j] == little[j] && (i + j) < len)
		{
			if (little[j + 1] == '\0')
				return ((char *) &big[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}

/*#include <stdio.h>
int main(void)
{
	const char *str = "Hello World !!!";
	char *res;

	printf("Chaîne de test : \"%s\"\n\n", str);

	res = ft_strnstr(str, "World", 15);
	printf("Test 1 (Trouvé) : %s\n", res);

	res = ft_strnstr(str, "World", 5);
	printf("Test 2 (Limite trop courte) : %s\n", res);

	res = ft_strnstr(str, "", 10);
	printf("Test 3 (string vide) : %s\n", res);

	res = ft_strnstr(str, "Bonjour", 30);
	printf("Test 4 (Inexistant) : %s\n", res);

	return (0);
}*/
