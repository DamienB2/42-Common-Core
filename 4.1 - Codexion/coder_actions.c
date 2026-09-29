/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:40:43 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/29 13:45:10 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coder_compile(t_coder *coder)
{
	set_last_compile_start(coder->sim,
		coder->id - 1, get_time_ms());
	print_state(coder, "is compiling");
	usleep(coder->compile_t * 1000);
}

void	coder_debug(t_coder *coder)
{
	print_state(coder, "is debugging");
	usleep(coder->debug_t * 1000);
}

void	coder_refactor(t_coder *coder)
{
	print_state(coder, "is refactoring");
	usleep(coder->refactor_t * 1000);
}

int	finish_compile(t_coder *coder)
{
	increment_compiles_done(coder->sim, coder->id - 1);
	if (all_compiles_done(coder->sim))
	{
		sim_stop_simulation(coder->sim);
		wake_all_dongles(coder->sim);
		return (1);
	}
	return (0);
}
