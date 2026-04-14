/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 10:34:57 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/14 10:52:01 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
char	*ft_strdup(const char *s)
{
	size_t	i;
	char	*dup;

	i = 0;
	while(s[i] != '\0')
		i++;
	dup = malloc(sizeof(char) * (i + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

/*#include <stdio.h>
int	main(void)
{
	char	*src = "bonjour les coupaings";
	printf("%s", ft_strdup(src));
}*/
