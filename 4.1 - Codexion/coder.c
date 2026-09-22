/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 16:12:59 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/22 19:17:58 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	get_dongles_order(t_coder *coder,
		t_dongle **first, t_dongle **second)
{
	if (coder->id % 2 == 0)
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
	else
	{
		*first = coder->left_dongle;
		*second = coder->right_dongle;
	}
}

static int	take_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	long		deadline;

	get_dongles_order(coder, &first, &second);
	deadline = get_last_compile_start(coder->sim, coder->id - 1)
		+ coder->burnout_t;
	if (!take_dongle(first, coder->id, deadline, coder->sim))
		return (0);
	print_state(coder, "has taken a dongle");
	if (!take_dongle(second, coder->id, deadline, coder->sim))
	{
		release_dongle(first, coder->sim->dongle_cd);
		return (0);
	}
	print_state(coder, "has taken a dongle");
	return (1);
}

static void	coder_compile(t_coder *coder)
{
	set_last_compile_start(coder->sim,
		coder->id - 1, get_time_ms());
	print_state(coder, "is compiling");
	usleep(coder->compile_t * 1000);
}

static void	coder_debug(t_coder *coder)
{
	print_state(coder, "is debugging");
	usleep(coder->debug_t * 1000);
}

static void	coder_refactor(t_coder *coder)
{
	print_state(coder, "is refactoring");
	usleep(coder->refactor_t * 1000);
}

static int	finish_compile(t_coder *coder)
{
	increment_compiles_done(coder->sim, coder->id - 1);
	if (all_compiles_done(coder->sim))
	{
		sim_stop_simulation(coder->sim);
		wake_all_dongles(coder->sim);
		return (1);
	}
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->left_dongle == coder->right_dongle)
		return (NULL);
	while (!sim_should_stop(coder->sim))
	{
		if (!take_both_dongles(coder))
			break ;
		coder_compile(coder);
		release_dongle(coder->left_dongle, coder->sim->dongle_cd);
		release_dongle(coder->right_dongle, coder->sim->dongle_cd);
		if (finish_compile(coder))
			break ;
		if (sim_should_stop(coder->sim))
			break ;
		coder_debug(coder);
		if (sim_should_stop(coder->sim))
			break ;
		coder_refactor(coder);
	}
	return (NULL);
}
