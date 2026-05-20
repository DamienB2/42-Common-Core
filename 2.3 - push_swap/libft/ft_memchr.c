/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:05:02 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/22 14:59:28 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	str = (unsigned char *) s;
	i = 0;
	while (i < n)
	{
		if (*str == (unsigned char)c)
			return ((void *) str);
		str++;
		i++;
	}
	return (NULL);
}

/*#include <stdio.h>
  int main(void)
  {
  char s[] = "salut";
  char c = 'a';

  char *result = ft_memchr(s, c, 2);

  if (result)
  printf("trouve: %s\n", result);
  else
  printf("pas trouve.\n");

  return (0);
  }*/
