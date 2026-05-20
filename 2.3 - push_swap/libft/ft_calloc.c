/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 16:44:28 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/24 11:28:12 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*tab;
	size_t	i;
	size_t	totalsize;

	if (size != 0 && (nmemb * size) / size != nmemb)
		return (NULL);
	totalsize = (size * nmemb);
	tab = malloc(totalsize);
	if (!tab)
		return (NULL);
	i = 0;
	while (i < totalsize)
	{
		tab[i] = 0;
		i++;
	}
	return (tab);
}

/*#include <stdio.h>
int main(void)
{
	int numItems = 15;
	int *myArray = calloc(numItems, sizeof(int));

	for(int i = 0; i < numItems; i++) {
		myArray[i] = i + 1;
	}

	for(int i = 0; i < numItems; i++) {
		printf("%d ", myArray[i]);
	}

	free(myArray);
	myArray = NULL;

}*/
