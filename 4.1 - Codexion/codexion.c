/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 15:07:59 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/15 11:57:54 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	enqueue(t_sim *sim, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		if (!sim->waiting[i].active)
		{
			sim->waiting[i].active = 1;
			sim->waiting[i].id = coder->id;
			sim->waiting[i].seq = sim->next_seq;
			sim->next_seq += 1;
			sim->waiting[i].deadline = get_time_ms() + coder->burnout_t;
			return (i);
		}
		i++;
	}
	return (-1);
}

static int	is_my_turn(t_sim *sim, int id)
{
	int	i;
	int	best;

	best = -1;
	i = 0;
	while (i < sim->nb_coders)
	{
		if (sim->waiting[i].active)
		{
			if (best == -1)
				best = i;
			else if (sim->scheduler == 0
				&& sim->waiting[i].seq < sim->waiting[best].seq)
				best = i;
			else if (sim->scheduler == 1
				&& sim->waiting[i].deadline < sim->waiting[best].deadline)
				best = i;
		}
		i++;
	}
	if (best == -1)
		return (0);
	return (sim->waiting[best].id == id);
}

void	*say(void *arg)
{
	t_coder	*coder;
	int		i;
	int		slot;

	coder = (t_coder *)arg;
	i = 0;
	while (i < coder->nb_compiles_required)
	{
		pthread_mutex_lock(&coder->sim->print_lock);
		slot = enqueue(coder->sim, coder);
		while (!is_my_turn(coder->sim, coder->id))
			pthread_cond_wait(&coder->sim->turn_cond, &coder->sim->print_lock);
		coder->sim->waiting[slot].active = 0;
		printf(
			"%ld, Hello from coder %d\n",
			get_sim_time(coder->sim->start_time), coder->id);
		printf(
			"%ld, print number %i\n",
			get_sim_time(coder->sim->start_time), coder->sim->nb_say);
		coder->sim->nb_say += 1;
		pthread_cond_broadcast(&coder->sim->turn_cond);
		pthread_mutex_unlock(&coder->sim->print_lock);
		i++;
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_params	params;

	if (!parser(argc, argv, &params))
		return (0);
	if (!simulation(params))
		return (1);
	return (0);
}