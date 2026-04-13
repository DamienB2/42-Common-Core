/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 12:00:16 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/13 13:07:46 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char    *ft_strrchr(const char *s, int c)
{
	int	i;
	
	i= 0;
	while (s[i] != '\0')
		i++;
	s += i;
        while (i != 0)
        {
                if (*s == c)
                        return ((char *)s);
                s--;
		i--;
        }
        return (0);
}

/*#include <stdio.h>
int main(void)
{
        char s[] = "salut twa";
        char c = 'a';

        char *result = ft_strrchr(s, c);

        if (result)
                printf("trouve: %s\n", result);
        else
                printf("pas trouve.\n");

        return (0);
}*/
