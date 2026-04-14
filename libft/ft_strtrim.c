/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 12:27:56 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/14 14:59:17 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

int	is_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*result;
	int	i;

	start = 0;
	end = ft_strlen(s1);

	if(!s1 || !set)
		return (NULL);

	while (s1[start] && is_set(s1[start], set))
		start++;
	while(start < end && is_set(s1[end - 1], set))
		end--;
	result = malloc(sizeof(char) * (end - start) + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (start < end)
	{
		result[i] = s1[start];
		i++;
		start++;
	}
	result[i] = '\0';
	return (result);
}

/*#include <stdio.h>
int main(void)
{
    char *s1 = "   ---Hello World!---   ";
    char *set = " -";
    char *res;

    printf("Chaîne originale : [%s]\n", s1);
    printf("Set à retirer    : [%s]\n", set);

    res = ft_strtrim(s1, set);

    if (res)
    {
        printf("Résultat trimé   : [%s]\n", res);
        free(res);
    }
    else
    {
        printf("\n❌ Erreur : Problème d'allocation (NULL).\n");
    }

    return (0);
}*/
