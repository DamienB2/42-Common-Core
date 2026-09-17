/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 14:49:27 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/11 by dbaise           ###   #######belgium.be         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_simulation(t_params params, t_sim *sim)
{
	int	i;

	sim->nb_coders = params.nb_coders;
	sim->nb_dongles = params.nb_coders;
	sim->nb_compiles_required = params.nb_compiles;
	sim->stop = 0;
	sim->scheduler = params.scheduler;
	sim->start_time = get_time_ms();
	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->stop_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_lock);
		return (0);
	}

	if (!init_dongles(sim))
	{
		pthread_mutex_destroy(&sim->stop_lock);
		pthread_mutex_destroy(&sim->print_lock);
		return (0);
	}
	sim->last_compile_start = malloc(sizeof(long) * sim->nb_coders);
	sim->nb_compiles_done = malloc(sizeof(int) * sim->nb_coders);
	if (!sim->last_compile_start || !sim->nb_compiles_done)
	{
		free(sim->last_compile_start);
		free(sim->nb_compiles_done);
		destroy_dongles(sim);
		pthread_mutex_destroy(&sim->stop_lock);
		pthread_mutex_destroy(&sim->print_lock);
		return (0);
	}
	init_counters(sim);
	return (1);
}

static void	init_counters(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		sim->last_compile_start[i] = sim->start_time;
		sim->nb_compiles_done = 0;
		i++;
	}
}

static int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->nb_dongles);
	if (!sim->dongles)
		return (0);
	i = 0;
	while (i < sim->dongles)
	{
		init_dongle(&sim->dongles[i], sim->scheduler);
		i++;
	}
	return (1);
}

static int	destroy_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->dongles)
	{
		destroy_dongle(&sim->dongles[i]);
		i++;
	}
	free(sim->dongles);
}

static void	assign_dongles(t_coder *coder, t_sim *sim, int index)
{
	coder->left_dongle = &sim->dongles[index];
	coder->right_dongle = &sim->dongles[(index + 1) % sim->nb_dongles];
}


static void	cleanup_simulation(t_sim *sim, t_coder *coders)
{
	destroy_dongles(sim);
	free(sim->last_compile_start);
	free(sim->nb_compiles_done);
	pthread_mutex_destroy(&sim->stop_lock);
	pthread_mutex_destroy(&sim->print_lock);
	free(coders);
}

static void	init_coders(t_coder *coders, t_params params, t_sim *sim)
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

static int	start_coders(t_coder *coders, int nb_coders, t_sim *sim)
{
	int	i;

	i = 0;
	while (i < nb_coders)
	{
		if (pthread_create(&coders[i].thread, NULL, say, &coders[i]) != 0)
		{
			sim_stop_simulation(sim);
			while (i >= 0)
			{
				pthread_join(&coders[i].thread, NULL);
				i--;
			}
		}
		i++;
	}
	return (1);
}

int	simulation(t_params params)
{
	t_coder	*coders;
	t_sim	sim;
	int		i;

	if (!init_simulation(params, &sim))
		return (0);
	coders = malloc(sizeof(t_coder) * params.nb_coders);
	if (!coders)
	{
		cleanup_simulation(&sim, NULL);
		return (0);
	}
	init_coders(coders, params, &sim); 
	if (!start_coders(coders, params.nb_coders, &sim))
	{
		cleanup_simulation(&sim, coders);
		return (0);
	}
	i = 0;
	while (i < params.nb_coders)
	{
		pthread_join(coders[i].thread, NULL);
		i++;
	}
	cleanup_simulation(&sim, coders);
	return (1);
}