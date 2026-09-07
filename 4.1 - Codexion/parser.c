/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 15:24:26 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/07 17:53:29 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int	args_checker(int argc, char *argv[])
{
	int	i;

	i = 1;
	if (argc != 9)
	{
		printf("%s", "The number of parameters is incorrect\n");
		return (0);
	}
	while (i < argc - 1)
	{
		if (!ft_is_str_digit(argv[i]))
		{
			printf(
				"argument %i (%s) is an incorrect parameter\n", i - 1, argv[i]);
			return (0);
		}
		i++;
	}
	if (strcmp(argv[i], "fifo") != 0 && strcmp(argv[i], "edf") != 0)
	{
		printf("incorrect parameter, it should be 'fifo' or 'edf'.\n");
		return (0);
	}
	return (1);
}

void	cast_and_set(int argc, char *argv[], t_params	*params)
{
	params->nb_coders = ft_atoi(argv[1]);
}

//a.out 4 200 200 200 200 3 200 fifo
int	parser(int argc, char *argv[], t_params	*params)
{
	(void) params;
	if (!args_checker(argc, argv))
		return (0);
	cast_and_set(argc, argv, params);
	return (1);
}