/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 15:16:39 by demain            #+#    #+#             */
/*   Updated: 2026/10/02 17:50:56 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	dongle_ready(t_dongle *dongle, int id)
{
	return (
		pq_peek_id(&dongle->queue) == id
		&& !dongle->taken
		&& get_time_ms() >= dongle->free_at);
}

static void	wait_for_turn(t_dongle *dongle, int id, t_sim *sim)
{
	while (!dongle_ready(dongle, id) && !sim_should_stop(sim))
	{
		if (pq_peek_id(&dongle->queue) == id && !dongle->taken)
		{
			pthread_mutex_unlock(&dongle->lock);
			usleep(500);
			pthread_mutex_lock(&dongle->lock);
		}
		else
			pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
}

int	take_dongle(t_dongle *dongle, int id, long deadline, t_sim *sim)
{
	t_queue_node	node;

	pthread_mutex_lock(&dongle->lock);
	node.id = id;
	node.seq = dongle->next_seq++;
	node.deadline = deadline;
	pq_push(&dongle->queue, node);
	wait_for_turn(dongle, id, sim);
	if (sim_should_stop(sim))
	{
		pq_remove_id(&dongle->queue, id);
		pthread_cond_broadcast(&dongle->cond);
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	pq_pop(&dongle->queue);
	dongle->taken = 1;
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}

void	release_dongle(t_dongle *dongle, int cooldown)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->taken = 0;
	dongle->free_at = get_time_ms() + cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

void	wake_all_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_dongles)
	{
		pthread_mutex_lock(&sim->dongles[i].lock);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].lock);
		i++;
	}
}
