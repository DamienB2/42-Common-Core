/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 15:16:39 by demain            #+#    #+#             */
/*   Updated: 2026/09/25 16:11:26 by dbaise           ###   #######belgium.be */
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

void	destroy_dongle(t_dongle *dongle)
{
	pthread_mutex_destroy(&dongle->lock);
	pthread_cond_destroy(&dongle->cond);
}

static void	build_wait_ts(struct timespec *ts, long wait_ms)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ts->tv_sec = tv.tv_sec;
	ts->tv_nsec = (tv.tv_usec * 1000) + (wait_ms * 1000000L);
	ts->tv_sec += ts->tv_nsec / 1000000000L;
	ts->tv_nsec %= 1000000000L;
}

int	take_dongle(t_dongle *dongle, int id, long deadline, t_sim *sim)
{
	t_queue_node	node;
	struct timespec	ts;

	pthread_mutex_lock(&dongle->lock);
	node.id = id;
	node.seq = dongle->next_seq++;
	node.deadline = deadline;
	pq_push(&dongle->queue, node);
	while (!dongle_ready(dongle, id) && !sim_should_stop(sim))
	{
		build_wait_ts(&ts, 5);
		pthread_cond_timedwait(&dongle->cond, &dongle->lock, &ts);
	}
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
