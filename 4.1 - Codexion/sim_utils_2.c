/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:03:23 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/21 16:28:54 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_last_compile_start(t_sim *sim, int index, long time)
{
	pthread_mutex_lock(&sim->counters_lock);
	sim->last_compile_start[index] = time;
	pthread_mutex_unlock(&sim->counters_lock);
}

long	get_last_compile_start(t_sim *sim, int index)
{
	long	value;

	pthread_mutex_lock(&sim->counters_lock);
	value = sim->last_compile_start[index];
	pthread_mutex_unlock(&sim->counters_lock);
	return (value);
}

void	increment_compiles_done(t_sim *sim, int index)
{
	pthread_mutex_lock(&sim->counters_lock);
	sim->nb_compiles_done[index] += 1;
	pthread_mutex_unlock(&sim->counters_lock);
}

int	all_compiles_done(t_sim *sim)
{
	int	index;
	int	all_done;

	pthread_mutex_lock(&sim->counters_lock);
	index = 0;
	all_done = 1;
	while (index < sim->nb_coders)
	{
		if (sim->nb_compiles_done[index] < sim->nb_compiles_required)
			all_done = 0;
		index++;
	}
	pthread_mutex_unlock(&sim->counters_lock);
	return (all_done);
}
