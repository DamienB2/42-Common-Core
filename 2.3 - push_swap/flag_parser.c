/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 18:39:56 by dmonseur          #+#    #+#             */
/*   Updated: 2026/05/20 16:30:43 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	flag_parser(char *s, t_count *ops_count)
{
	if (ft_strncmp(s, "--simple", 8) == 0 && s[8] == '\0'
		&& ops_count->flag == 0)
		return (ops_count->flag++, ops_count->simple++, 1);
	else if (ft_strncmp(s, "--medium", 8) == 0 && s[8] == '\0'
		&& ops_count->flag == 0)
		return (ops_count->flag++, ops_count->medium++, 1);
	else if (ft_strncmp(s, "--complex", 9) == 0 && s[9] == '\0'
		&& ops_count->flag == 0)
		return (ops_count->flag++, ops_count->complex++, 1);
	else if (ft_strncmp(s, "--adaptive", 10) == 0 && s[10] == '\0'
		&& ops_count->flag == 0)
		return (ops_count->flag++, ops_count->adaptive++, 1);
	else if (ft_strncmp(s, "--bench", 7) == 0 && s[7] == '\0'
		&& ops_count->bench == 0)
		return (ops_count->bench++, 1);
	return (0);
}

int	validate_args_2(char **argv, int i)
{
	char	**split;
	int		j;

	if (!is_number(argv[i]))
	{
		split = ft_split(argv[i], ' ');
		if (!split)
			return (1);
		j = 0;
		while (split[j])
		{
			if (!is_number(split[j]))
			{
				free_tab(split);
				return (1);
			}
			j++;
		}
		free_tab(split);
	}
	return (0);
}

int	validate_args(int argc, char **argv, t_count *ops_count)
{
	int		i;

	i = 1;
	while (i < argc)
	{
		if (flag_parser(argv[i], ops_count))
		{
			i++;
			continue ;
		}
		if (validate_args_2(argv, i))
			return (1);
		i++;
	}
	return (0);
}
