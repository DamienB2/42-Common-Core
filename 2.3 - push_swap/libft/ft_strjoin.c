/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 11:59:46 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/17 18:34:29 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	fill_result(char *result, char const *s, int pos)
{
	int	i;

	i = 0;
	while (s[i])
	{
		result[pos] = s[i];
		pos++;
		i++;
	}
	return (pos);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	total_len;
	char	*result;
	int		pos;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	total_len = s1_len + s2_len;
	result = malloc(sizeof(char) * (total_len + 1));
	if (!result)
		return (NULL);
	pos = 0;
	pos = fill_result(result, s1, pos);
	pos = fill_result(result, s2, pos);
	result[pos] = '\0';
	return (result);
}

/*#include <stdio.h>
  int main(void)
  {
  char *s1 = "Hello ";
  char *s2 = "world!";
  char *res;

  res = ft_strjoin(s1, s2);

  if (res)
  {
  printf("Résultat : %s\n", res);
  free(res);
  }
  else
  {
  printf("Erreur d'allocation.\n");
  }

  return (0);
  }*/
