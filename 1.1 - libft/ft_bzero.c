/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 13:02:12 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/17 15:56:37 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*p;
	size_t			i;

	i = 0;
	p = (unsigned char *) s;
	while (i < n)
	{
		p[i] = '\0';
		i++;
	}
}

/*#include <stdio.h>
int main() {
    char str[] = "Hello World";
    char str1[] = "Hello World";

    bzero(str1, 3);
    ft_bzero(str, 3);
    
    printf("Resultat : %s\n", str1);

    printf("Resultat ft_bzero: %s\n", str);

    return 0;
}*/
