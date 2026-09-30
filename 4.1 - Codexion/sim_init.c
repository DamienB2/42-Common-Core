/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 11:44:45 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/30 14:04:28 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_sim_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->stop_lock, NULL) != 0)
		return (pthread_mutex_destroy(&sim->print_lock), 0);
	if (pthread_mutex_init(&sim->counters_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->stop_lock);
		pthread_mutex_destroy(&sim->print_lock);
		return (0);
	}
	return (1);
}

int	init_simulation(t_params params, t_sim *sim)
{
	sim->nb_coders = params.nb_coders;
	sim->nb_dongles = params.nb_coders;
	sim->nb_compiles_required = params.nb_compiles;
	sim->stop = 0;
	sim->scheduler = params.scheduler;
	sim->burnout_t = params.burnout_t;
	sim->dongle_cd = params.dongle_cd;
	sim->start_time = get_time_ms();
	if (!init_sim_mutexes(sim))
		return (0);
	if (!init_dongles(sim))
		return (pthread_mutex_destroy(&sim->counters_lock),
			pthread_mutex_destroy(&sim->stop_lock),
			pthread_mutex_destroy(&sim->print_lock), 0);
	sim->last_compile_start = malloc(sizeof(long) * sim->nb_coders);
	sim->nb_compiles_done = malloc(sizeof(int) * sim->nb_coders);
	if (!sim->last_compile_start || !sim->nb_compiles_done)
		return (cleanup_simulation(sim, NULL), 0);
	init_counters(sim);
	return (1);
}

void	init_counters(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		sim->last_compile_start[i] = sim->start_time;
		sim->nb_compiles_done[i] = 0;
		i++;
	}
}

int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->nb_dongles);
	if (!sim->dongles)
		return (0);
	i = 0;
	while (i < sim->nb_dongles)
	{
		init_dongle(&sim->dongles[i], sim->scheduler);
		i++;
	}
	return (1);
}

void	init_coders(t_coder *coders, t_params params, t_sim *sim)
{
	int	i;

	i = 0;
	while (i < params.nb_coders)
	{
		coders[i].id = i + 1;
		coders[i].compile_t = params.compile_t;
		coders[i].debug_t = params.debug_t;
		coders[i].refactor_t = params.refactor_t;
		coders[i].burnout_t = params.burnout_t;
		coders[i].sim = sim;
		assign_dongles(&coders[i], sim, i);
		i++;
	}
}
