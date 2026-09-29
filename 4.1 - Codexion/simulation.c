/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 15:55:40 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/29 20:20:54 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	assign_dongles(t_coder *coder, t_sim *sim, int index)
{
	coder->left_dongle = &sim->dongles[index];
	coder->right_dongle = &sim->dongles[(index + 1) % sim->nb_dongles];
}

int	start_coders(t_coder *coders, int nb_coders, t_sim *sim)
{
	int	i;

	i = 0;
	while (i < nb_coders)
	{
		if (pthread_create(
				&coders[i].thread, NULL, coder_routine, &coders[i]) != 0)
		{
			sim_stop_simulation(sim);
			wake_all_dongles(sim);
			i--;
			while (i >= 0)
			{
				pthread_join(coders[i].thread, NULL);
				i--;
			}
			return (0);
		}
		i++;
	}
	return (1);
}

static void	join_and_cleanup(t_sim *sim, t_coder *coders, t_params params)
{
	int	index;

	index = 0;
	while (index < params.nb_coders)
	{
		pthread_join(coders[index].thread, NULL);
		index++;
	}
	sim_stop_simulation(sim);
	wake_all_dongles(sim);
	pthread_join(sim->monitor_thread, NULL);
	cleanup_simulation(sim, coders);
}

int	simulation(t_params params)
{
	t_coder	*coders;
	t_sim	sim;

	if (params.nb_compiles == 0)
		return (1);
	if (!init_simulation(params, &sim))
		return (0);
	coders = malloc(sizeof(t_coder) * params.nb_coders);
	if (!coders)
		return (cleanup_simulation(&sim, NULL), 0);
	init_coders(coders, params, &sim);
	if (pthread_create(&sim.monitor_thread, NULL,
			monitor_routine, &sim) != 0)
		return (cleanup_simulation(&sim, coders), 0);
	if (!start_coders(coders, params.nb_coders, &sim))
	{
		sim_stop_simulation(&sim);
		pthread_join(sim.monitor_thread, NULL);
		return (cleanup_simulation(&sim, coders), 0);
	}
	join_and_cleanup(&sim, coders, params);
	return (1);
}
