/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 14:49:27 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/11 17:00:15 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	simulation(t_params params)
{
	t_coder	*coders;
	t_sim	sim;
	int		i;

	init_simulation(&sim, params);
	init_coders(&coders, sim);

	sim.start_time = get_time_ms();
	i = 0;
	while (i < params.nb_coders)
	{
		coders[i].id = i + 1;
		coders[i].sim = &sim;
		if (pthread_create(&coders[i].thread, NULL, say, &coders[i]) != 0)
		{
			pthread_mutex_destroy(&sim.print_lock);
			free(coders);
			return (0);
		}
		i++;
	}
	i = 0;
	while (i < params.nb_coders)
	{
		pthread_join(coders[i].thread, NULL);
		i++;
	}
	pthread_mutex_destroy(&sim.print_lock);
	free(coders);
	return (1);
}

void	init_simulation(t_sim *sim, t_params params)
{
	int	i;

	sim->nb_coders = params.nb_coders;
	sim->nb_compiles_required = params.nb_compiles;
	sim->next_seq = 0;
	sim->scheduler = params.scheduler;
	sim->waiting = malloc(sizeof(t_request) * params.nb_coders);

	i = 0;
	while (i < sim->nb_coders)
	{
		sim->waiting[i].active = 0;
		i++;
	}
}

void	init_coder(t_coder *coders, t_sim sim)
{
	coders = malloc(sizeof(t_coder) * sim.nb_coders);
	if (!coders)
		return (0);
	if (pthread_mutex_init(&sim.print_lock, NULL) != 0)
	{
		free(coders);
		return (0);
	}
}