/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 16:12:59 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/21 17:29:18 by dbaise           ###   #######belgium.be */
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

static void	release_both_dongles(t_coder *coder)
{
	release_dongle(coder->left_dongle, coder->sim->dongle_cd);
	release_dongle(coder->right_dongle, coder->sim->dongle_cd);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_dongle	*first;
	t_dongle	*second;
	long		deadline;

	coder = (t_coder *)arg;
	if (coder->left_dongle == coder->right_dongle)
		return (NULL);
	get_dongles_order(coder, &first, &second);
	deadline = get_last_compile_start(coder->sim, coder->id - 1)
		+ coder->burnout_t;
	if (!take_dongle(first, coder->id, deadline, coder->sim))
		return (NULL);
	print_state(coder, "has taken a dongle");
	if (!take_dongle(second, coder->id, deadline, coder->sim))
	{
		release_dongle(first, coder->sim->dongle_cd);
		return (NULL);
	}
	print_state(coder, "has taken a dongle");
	usleep(20000);
	release_both_dongles(coder);
	return (NULL);
}
