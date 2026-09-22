/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:14:24 by demain            #+#    #+#             */
/*   Updated: 2026/09/22 19:13:52 by dbaise           ###   #######belgium.be */
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
	pthread_mutex_destroy(&sim->counters_lock);
	pthread_mutex_destroy(&sim->stop_lock);
	pthread_mutex_destroy(&sim->print_lock);
	free(coders);
}

void	print_state(t_coder *coder, char *state)
{
	pthread_mutex_lock(&coder->sim->print_lock);
	if (!sim_should_stop(coder->sim))
	{
		printf("%ld %d %s\n",
			get_sim_time(coder->sim->start_time),
			coder->id,
			state);
	}
	pthread_mutex_unlock(&coder->sim->print_lock);
}

void	print_death(t_sim *sim, int id)
{
	pthread_mutex_lock(&sim->print_lock);
	if (!sim_should_stop(sim))
	{
		sim_stop_simulation(sim);
		printf("%ld %d burned out\n", get_sim_time(sim->start_time), id);
	}
	pthread_mutex_unlock(&sim->print_lock);
	wake_all_dongles(sim);
}
