/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strat_selector.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 15:49:06 by dmonseur          #+#    #+#             */
/*   Updated: 2026/05/20 15:52:33 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	adaptative_strat(t_stack *stack_a, t_stack *stack_b)
{
	double	chaos;

	chaos = stack_a->chaos;
	stack_a->ops_count.adaptive++;
	if (chaos < 0.2)
		stack_a->ops_count.simple++;
	if (chaos >= 0.2 && chaos < 0.5)
		stack_a->ops_count.medium++;
	if (chaos >= 0.5)
		stack_a->ops_count.complex++;
	strat_selector(stack_a, stack_b);
}

void	strat_selector(t_stack *stack_a, t_stack *stack_b)
{
	if (stack_a->ops_count.simple == 1)
		insertion_sort(stack_a, stack_b);
	if (stack_a->ops_count.medium == 1)
		chunk_sort(stack_a, stack_b);
	if (stack_a->ops_count.complex == 1)
		radix_sort(stack_a, stack_b);
	if ((stack_a->ops_count.adaptive == 1
			&& stack_a->ops_count.selected_strat == 0)
		|| (stack_a->ops_count.simple == 0
			&& stack_a->ops_count.medium == 0
			&& stack_a->ops_count.complex == 0))
	{
		stack_a->ops_count.selected_strat ++;
		adaptative_strat(stack_a, stack_b);
	}
}
