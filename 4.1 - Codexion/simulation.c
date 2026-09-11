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

/*
** Prepare tout l'etat partage de la simulation : mutex, cond var,
** compteurs et tableau de salle d'attente. Ne touche pas aux coders.
** Retourne 1 si tout est pret, 0 si une allocation/init a echoue
** (et nettoie ce qui a deja ete initialise dans ce cas).
*/
int	init_simulation(t_params params, t_sim *sim)
{
	int	i;

	sim->nb_coders = params.nb_coders;
	sim->scheduler = params.scheduler;
	sim->nb_say = 0;
	sim->next_seq = 0;
	sim->start_time = get_time_ms();
	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
		return (0);
	if (pthread_cond_init(&sim->turn_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_lock);
		return (0);
	}
	sim->waiting = malloc(sizeof(t_request) * params.nb_coders);
	if (!sim->waiting)
	{
		pthread_cond_destroy(&sim->turn_cond);
		pthread_mutex_destroy(&sim->print_lock);
		return (0);
	}
	i = 0;
	while (i < params.nb_coders)
	{
		sim->waiting[i].active = 0;
		i++;
	}
	return (1);
}

static void	cleanup_simulation(t_sim *sim, t_coder *coders)
{
	pthread_cond_destroy(&sim->turn_cond);
	pthread_mutex_destroy(&sim->print_lock);
	free(sim->waiting);
	free(coders);
}

static int	init_coders(t_coder *coders, t_params params, t_sim *sim)
{
	int	i;

	i = 0;
	while (i < params.nb_coders)
	{
		coders[i].id = i + 1;
		coders[i].nb_compiles_required = params.nb_compiles;
		coders[i].compile_t = params.compile_t;
		coders[i].debug_t = params.debug_t;
		coders[i].refactor_t = params.refactor_t;
		coders[i].burnout_t = params.burnout_t;
		coders[i].sim = sim;
		if (pthread_create(&coders[i].thread, NULL, say, &coders[i]) != 0)
			return (0);
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
	if (!init_coders(coders, params, &sim))
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