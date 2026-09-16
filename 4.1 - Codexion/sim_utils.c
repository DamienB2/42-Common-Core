/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demain <demain@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:14:24 by demain            #+#    #+#             */
/*   Updated: 2026/09/16 16:23:02 by demain           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sim_should_stop(t_sim *sim)
{
	int value;

	pthread_mutex_lock(&sim->stop_lock);
	value = sim->stop;
	pthread_mutex_unlock(&sim->stop_lock);
	return (value);
}

void	sim_stop_simulation(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_lock);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_lock);
}
