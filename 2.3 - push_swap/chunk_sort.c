/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 16:53:12 by dmonseur          #+#    #+#             */
/*   Updated: 2026/05/20 16:07:22 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	insertion_sort_reverse(t_stack *stack_a, t_stack *stack_b)
{
	int	max;

	max = stack_b->actual_size - 1;
	while (max >= 0)
	{
		while (1)
		{
			if (stack_b->numbers[0].index == max)
			{
				handle_push(stack_a, stack_b, "pa");
				break ;
			}
			if (find_direction(stack_b, max))
				handle_rotate_up(stack_a, stack_b, "rb");
			else
				handle_rotate_down(stack_a, stack_b, "rrb");
		}
		max--;
	}
}

int	chunck_sort2(t_stack *stack_a, t_stack *stack_b, int chunck[2], int i)
{
	if (stack_a->numbers[0].index >= chunck[0]
		&& stack_a->numbers[0].index < chunck[1])
	{
		handle_push(stack_a, stack_b, "pb");
		i++;
	}
	else
		handle_rotate_up(stack_a, stack_b, "ra");
	return (i);
}

void	chunk_sort(t_stack *stack_a, t_stack *stack_b)
{
	int	i;
	int	chunk_size;
	int	chunck[2];
	int	max;

	max = stack_a->actual_size;
	chunk_size = ft_sqrt(max);
	chunck[0] = 0;
	chunck[1] = chunk_size;
	i = 0;
	if (is_stack_sort(stack_a))
		return ;
	while (i < max)
	{
		if (i >= chunck[1])
		{
			chunck[0] += chunk_size;
			chunck[1] += chunk_size;
			if (i >= max - chunk_size)
				chunck[1] = max;
		}
		i = chunck_sort2(stack_a, stack_b, chunck, i);
	}
	insertion_sort_reverse(stack_a, stack_b);
}
