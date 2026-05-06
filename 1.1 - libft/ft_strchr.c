/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:13:48 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/22 15:01:01 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (1)
	{
		if (*s == (char)c)
			return ((char *) s);
		if (*s == '\0')
			break ;
		s++;
	}
	return (NULL);
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
