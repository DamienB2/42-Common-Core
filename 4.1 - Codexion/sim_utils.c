/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:14:24 by demain            #+#    #+#             */
/*   Updated: 2026/09/18 11:50:51 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sim_should_stop(t_sim *sim)
{
	int	value;

	pthread_mutex_lock(&sim->stop_lock);
	value = sim->stop;
	pthread_mutex_unlock(&sim->stop_lock);
	return (value);
}

void	sim_stop_simulation(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_lock);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_lock);
}

void	cleanup_simulation(t_sim *sim, t_coder *coders)
{
	destroy_dongles(sim);
	free(sim->last_compile_start);
	free(sim->nb_compiles_done);
	pthread_mutex_destroy(&sim->stop_lock);
	pthread_mutex_destroy(&sim->print_lock);
	free(coders);
}
