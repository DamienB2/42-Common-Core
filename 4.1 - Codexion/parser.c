/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 15:24:26 by dbaise            #+#    #+#             */
/*   Updated: 2026/08/25 18:26:37 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//a.out 4 200 200 200 200 3 200 fifo
int	parser(int argc, char *argv[], t_params	*params)
{
	if (argc != 9)
		return (0);

	if ()
	{
	}

	return (0);
}

int	check_digit_exept_last(int argc, char *argv[], t_params *params)
{
	int	i;
	int	*ptr;

	i = 1;
	ptr = (int *)params;
	while (i < argc - 1)
	{	}
	return (0);
}