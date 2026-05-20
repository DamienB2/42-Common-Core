/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 14:20:17 by dmonseur          #+#    #+#             */
/*   Updated: 2026/05/11 18:07:22 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	swap(t_stack *stack)
{
	t_num	tmp;

	if (!stack || stack->actual_size < 2)
		return ;
	tmp = stack->numbers[0];
	stack->numbers[0] = stack->numbers[1];
	stack->numbers[1] = tmp;
}

void	push(t_stack *dest, t_stack *src)
{
	t_num	tmp;
	int		i;

	if (src->actual_size == 0)
		return ;
	tmp = src->numbers[0];
	i = 1;
	while (i < src->actual_size)
	{
		src->numbers[i - 1] = src->numbers[i];
		i++;
	}
	src->actual_size--;
	i = dest->actual_size;
	while (i > 0)
	{
		dest->numbers[i] = dest->numbers[i - 1];
		i--;
	}
	dest->numbers[0] = tmp;
	dest->actual_size++;
}

void	rotate_up(t_stack *stack)
{
	t_num	tmp;
	int		i;

	if (stack->actual_size < 2)
		return ;
	tmp = stack->numbers[0];
	i = 0;
	while (i < stack->actual_size - 1)
	{
		stack->numbers[i] = stack->numbers[i + 1];
		i++;
	}
	stack->numbers[i] = tmp;
}

void	rotate_down(t_stack *stack)
{
	t_num	tmp;
	int		i;

	if (stack->actual_size < 2)
		return ;
	i = stack->actual_size - 1;
	tmp = stack->numbers[i];
	while (i > 0)
	{
		stack->numbers[i] = stack->numbers[i - 1];
		i--;
	}
	stack->numbers[0] = tmp;
}
