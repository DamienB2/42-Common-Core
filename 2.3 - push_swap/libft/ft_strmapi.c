/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 15:30:30 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/20 10:40:13 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str;
	size_t	len;
	size_t	i;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*#include <stdio.h>

  char f(unsigned int i, char c)
  {
  char str;
  (void) i;
  str = c + 1;
  return (str);
  }

  int main()
  {
  char str1[] = "abc";
  char* str2;
  str2 = ft_strmapi(str1, *f);
  printf("%s\n", str2);
  }*/
