/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 11:25:40 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/30 13:43:55 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	get_sim_time(long simulation_time)
{
	return (get_time_ms() - simulation_time);
}

void	build_wait_ts(struct timespec *ts, long wait_ms)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ts->tv_sec = tv.tv_sec;
	ts->tv_nsec = (tv.tv_usec * 1000) + (wait_ms * 1000000L);
	ts->tv_sec += ts->tv_nsec / 1000000000L;
	ts->tv_nsec %= 1000000000L;
}
