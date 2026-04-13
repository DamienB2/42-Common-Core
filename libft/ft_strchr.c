/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:13:48 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/13 13:01:41 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == c)
			return ((char *)s);
		s++;
	}
	return (0);
}

/*#include <stdio.h>
int main(void)
{
	char s[] = "salut";
	char c = 'a';

	char *result = ft_strchr(s, c);

	if (result)
		printf("trouve: %s\n", result);
	else
		printf("pas trouve.\n");

	return (0);
}*/
