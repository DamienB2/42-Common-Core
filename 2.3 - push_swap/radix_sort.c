/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   radix_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 11:40:46 by dbaise            #+#    #+#             */
/*   Updated: 2026/05/18 13:26:44 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	radix_sort(t_stack *stack_a, t_stack *stack_b)
{
	int	i;
	int	bit;
	int	max;

	bit = 0;
	if (stack_a->actual_size <= SIMPLE_SCALE)
		insertion_sort(stack_a, stack_b);
	while (!is_stack_sort(stack_a))
	{
		i = 0;
		max = stack_a->actual_size;
		while (i < max && !is_stack_sort(stack_a))
		{
			if ((stack_a->numbers[0].index >> bit) & 1)
				handle_rotate_up(stack_a, stack_b, "ra");
			else
				handle_push(stack_a, stack_b, "pb");
			i++;
		}
		while (stack_b->actual_size > 0)
		{
			handle_push(stack_a, stack_b, "pa");
		}
		bit++;
	}
}
