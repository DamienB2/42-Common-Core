/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 13:09:54 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/13 15:02:13 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	
	while((s1[i] != '\0' || s2[i] != '\0') && i < n)
	{
		if (s1[i] == s2[i])
			i++;
		else
			return (s1[i] - s2[i]);
	}
	return (0);
}

/*#include <stdio.h>
int     main(void)
{
        char *str1 = "Hello";
        char *str2 = "Hello";
        char *str3 = "Hallo";
        char *str4 = "Hello World";

        printf("Test 1 (Identiques) : %d\n", ft_strncmp(str1, str2, 3));

        printf("Test 2 (Différents) : %d\n", ft_strncmp(str1, str3, 3));

        printf("Test 3 (S1 court)   : %d\n", ft_strncmp(str1, str4, 7));

        return (0);
}*/
