/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 15:19:30 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/17 17:45:33 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*get_word(char const *s, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc(sizeof(char) * (end - start + 1));
	if (!word)
		return (NULL);
	while (start < end)
	{
		word[i] = s[start];
		i++;
		start++;
	}
	word[i] = '\0';
	return (word);
}

static int	count_words(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

static void	*free_tab(char **tab, int i)
{
	while (i >= 0)
	{
		free(tab[i]);
		i--;
	}
	free(tab);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	int		start;
	char	**tab;

	tab = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!s || !tab)
		return (NULL);
	i = 0;
	j = 0;
	start = -1;
	while (i <= (int)ft_strlen(s))
	{
		if (s[i] != c && start < 0)
			start = i;
		else if ((s[i] == c || s[i] == '\0') && start >= 0)
		{
			tab[j] = get_word(s, start, i);
			if (!tab[j++])
				return (free_tab(tab, j - 1));
			start = -1;
		}
		i++;
	}
	tab[j] = NULL;
	return (tab);
}

#include <stdio.h>
  int	main(void)
  {
  char	**tab;
  int		i;

  i = 0;
  tab = ft_split(" Salut--les-amis! ", '-');
  if (!tab)
  return (1);
  while (tab[i])
  {
  printf("Mot %d : [%s]\n", i, tab[i]);
  i++;
  }
  return (0);
  }
