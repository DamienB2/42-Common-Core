/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insertion_sort.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 10:26:30 by dbaise            #+#    #+#             */
/*   Updated: 2026/05/20 15:55:58 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	insertion_sort(t_stack *stack_a, t_stack *stack_b)
{
	int	i;
	int	max;

	max = stack_a->actual_size;
	i = 0;
	while (i < max && !is_stack_sort(stack_a))
	{
		while (stack_a->numbers[0].index != i)
		{
			if (find_direction(stack_a, i))
				handle_rotate_up(stack_a, stack_b, "ra");
			else
				handle_rotate_down(stack_a, stack_b, "rra");
		}
		if (!is_stack_sort(stack_a))
			handle_push(stack_a, stack_b, "pb");
		i++;
	}
	while (stack_b->actual_size > 0)
		handle_push(stack_a, stack_b, "pa");
}

int	find_direction(t_stack *stack_a, int index_to_find)
{
	int	cpt;

	cpt = 0;
	while (cpt < stack_a->actual_size)
	{
		if (stack_a->numbers[cpt].index == index_to_find)
			break ;
		cpt++;
	}
	if (cpt <= stack_a->actual_size / 2)
		return (1);
	else
		return (0);
}
