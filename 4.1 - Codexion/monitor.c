/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 19:23:41 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/22 19:30:39 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	find_dead_coder(t_sim *sim)
{
	int		index;
	int		id;
	long 	now;

	pthread_mutex_lock(&sim->counters_lock);
	now = get_time_ms();
	index = 0;
	id = 0;
	while (index < sim->nb_coders)
	{
		if (now >= sim->last_compile_start[index] + sim->burnout_t)
		{
			id = index + 1;
			break ;
		}
		index++;
	}
	pthread_mutex_unlock(&sim->counters_lock);
	return (id);
}


void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		dead_coder_id;

	sim = (t_sim *) arg;
	while (!sim_should_stop(sim))
	{
		dead_coder_id = find_dead_coder(sim);
		if (dead_coder_id != 0)
		{
			print_death(sim, dead_coder_id);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}