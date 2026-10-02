/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 13:33:53 by dbaise            #+#    #+#             */
/*   Updated: 2026/10/02 17:53:45 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_dongle(t_dongle *dongle, int scheduler)
{
	dongle->taken = 0;
	dongle->free_at = 0;
	dongle->next_seq = 0;
	dongle->queue.size = 0;
	dongle->queue.scheduler = scheduler;
	pthread_mutex_init(&dongle->lock, NULL);
	pthread_cond_init(&dongle->cond, NULL);
}

//faire en sorte que destroy dongle se passe dans 
//sim_utils pour avoir la possibilité de déplacer les fonctions de dongle.c ici
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

void	destroy_dongle(t_dongle *dongle)
{
	pthread_mutex_destroy(&dongle->lock);
	pthread_cond_destroy(&dongle->cond);
}
