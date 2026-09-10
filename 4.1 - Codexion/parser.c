/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 15:24:26 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/10 15:54:15 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

static const char	*get_param_name(int index)
{
	if (index == 0)
		return ("number_of_coders");
	if (index == 1)
		return ("time_to_burnout");
	if (index == 2)
		return ("time_to_compile");
	if (index == 3)
		return ("time_to_debug");
	if (index == 4)
		return ("time_to_refactor");
	if (index == 5)
		return ("number_of_compiles_required");
	return ("dongle_cooldown");
}

static int	check_numeric_args(char *argv[])
{
	int		i;

	i = 0;
	while (i < 7)
	{
		if (!ft_is_str_digit(argv[i + 1]))
		{
			printf("'%s' must be a positive integer\n", get_param_name(i));
			return (0);
		}
		i++;
	}
	return (1);
}

static int	check_values(t_params *params)
{
	if (params->nb_coders <= 0)
	{
		printf("number_of_coders is incorrect\n");
		return(0);
	}
	if (params->burnout_t <= 0 || params->compile_t <= 0
		|| params->debug_t <= 0 || params->refactor_t <= 0)
	{
		printf("time values is incorrect\n");
		return(0);
	}
	if (params->nb_compiles <= 0)
	{
		printf("number_of_compiles_required is incorrect\n");
		return(0);
	}
	if (params->dongle_cd < 0)
	{
		printf("dongle_cooldown is incorrect\n");
		return(0);
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
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
    {
        printf("scheduler must be 'fifo' or 'edf'\n");
        return ;
    }
	if (strcmp(argv[8], "fifo") == 0)
		params->scheduler = 0;
	else
		params->scheduler = 1;
}

//a.out 4 200 200 200 200 3 200 fifo
int	parser(int argc, char *argv[], t_params	*params)
{
    if (argc != 9)
    {
		printf("wrong number of arguments (expected 8)\n");
        return (0);
    }
	if (!check_numeric_args(argv))
		return (0);
	atoi_and_set(argv, params);
	if (!check_values(params))
		return (0);
	return (1);
}