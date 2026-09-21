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

void	destroy_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_dongles)
	{
		destroy_dongle(&sim->dongles[i]);
		i++;
	}
	free(sim->dongles);
}

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
		if (pthread_create(&coders[i].thread, NULL, coder_routine, &coders[i]) != 0)
		{
			sim_stop_simulation(sim);
			wake_all_dongles(sim);
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
