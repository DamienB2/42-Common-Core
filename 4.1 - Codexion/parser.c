/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 15:24:26 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/08 13:01:28 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int	args_checker(int argc, char *argv[])
{
	int	i;

	i = 1;
	if (argc != 9)
	{
		printf("%s", "Error: the number of parameters is incorrect\n");
		return (0);
	}
	while (i < argc - 1)
	{
		if (!ft_is_str_digit(argv[i]))
		{
			printf("Error: invalid parameter\n");
			return (0);
		}
		i++;
	}
	if (strcmp(argv[i], "fifo") != 0 && strcmp(argv[i], "edf") != 0)
	{
		printf("Error: invalid parameter, it should be 'fifo' or 'edf'.\n");
		return (0);
	}
	return (1);
}

void	atoi_and_set(char *argv[], t_params	*params)
{
	params->nb_coders = ft_atoi(argv[1]);
	params->burnout_t = ft_atoi(argv[2]);
	params->compile_t = ft_atoi(argv[3]);
	params->debug_t = ft_atoi(argv[4]);
	params->refactor_t = ft_atoi(argv[5]);
	params->nb_compiles = ft_atoi(argv[6]);
	params->dongle_cd = ft_atoi(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		params->scheduler = 0;
	else
		params->scheduler = 1;
}

void	check_params(t_params *params)
{
	if (params->nb_coders == -1 || params->burnout_t == -1
		|| params->compile_t == -1 || params->debug_t == -1
		|| params->refactor_t == -1 || params->nb_compiles == -1
		|| params->dongle_cd == -1)
	{
		printf("Error: invalid parameter\n");
	}
}

//a.out 4 200 200 200 200 3 200 fifo
int	parser(int argc, char *argv[], t_params	*params)
{
	(void) params;
	if (!args_checker(argc, argv))
		return (0);
	atoi_and_set(argv, params);
	check_params(params);
	return (1);
}