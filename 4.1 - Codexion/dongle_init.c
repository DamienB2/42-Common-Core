/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 13:33:53 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/28 15:50:39 by dbaise           ###   #######belgium.be */
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