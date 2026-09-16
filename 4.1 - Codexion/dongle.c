/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demain <demain@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 15:16:39 by demain            #+#    #+#             */
/*   Updated: 2026/09/16 16:33:17 by demain           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    init_dongle(t_dongle *dongle, int scheduler)
{
	dongle->taken = 0;
	dongle->free_at = 0;
	dongle->next_seq = 0;
	dongle->queue.size = 0;
	dongle->queue.scheduler = scheduler;
	pthread_mutex_init(&dongle->lock, NULL);
	pthread_cond_init(&dongle->cond, NULL);
}

void destroy_dongle(t_dongle *dongle)
{
	pthread_mutex_destroy(&dongle->lock);
	pthread_cond_destroy(&dongle->cond);
}

static int dongle_ready(t_dongle *dongle, int id)
{
	return(
		pq_peek_id(&dongle->queue) == id
		&& !dongle->taken
		&& get_time_ms() >= dongle->free_at);
}

void take_dongle(t_dongle *dongle, int id, long deadline, t_sim *sim)
{
	t_queue_node node;

	pthread_mutex_lock(&dongle->lock);
	node.id = id;
	node.seq = dongle->next_seq++;
	node.deadline = deadline;
	pq_push(&dongle->queue, node);
	while(!dongle_ready(dongle, id) && !sim_should_stop(sim))
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	if(!sim_should_stop(sim))
	{
		pq_pop(&dongle->queue);
		dongle->taken = 1;
	}
	pthread_mutex_unlock(&dongle->lock);
}

void	release_dongle(t_dongle *dongle, int cooldown)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->taken = 0;
	dongle->free_at = get_time_ms() + cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}