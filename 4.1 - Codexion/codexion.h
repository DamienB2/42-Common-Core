/* ************************************************************************** */
/*		                                                                      */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 14:47:02 by dbaise            #+#    #+#             */
/*   Updated: 2026/08/25 15:02:25 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

typedef struct params
{
	int		nb_coders;		//number_of_coders
	int		burnout_t;		//time_to_burnout
	int		compile_t;		//time_to_compile
	int		debug_t;		//time_to_debug
	int		refactor_t;		//time_to_refactor
	int		nb_compiles;	//number_of_compiles_required
	int		dongle_cd;		//dongle_cooldown
	int		scheduler;		//scheduler 0 is fifo and 1 is edf
}	t_params;

//	parser.c
int	parser(int argc, char *argv[], t_params	params);

//	helper.c
int	ft_isdigit(int c);



#endif