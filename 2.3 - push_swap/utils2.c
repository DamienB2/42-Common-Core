/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:11:35 by dbaise            #+#    #+#             */
/*   Updated: 2026/05/20 13:18:45 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_sqrt(int nb)
{
	int	i;

	i = 0;
	while (i * i <= nb)
		i++;
	return (i - 1);
}

int	is_valid_int(char *str)
{
	long	nb;
	int		sign;
	int		i;

	i = 0;
	sign = 1;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	nb = 0;
	while (str[i])
	{
		nb = nb * 10 + (str[i] - '0');
		if (sign == 1 && nb > 2147483647)
			return (0);
		if (sign == -1 && nb > 2147483648)
			return (0);
		i++;
	}
	return (1);
}

int	is_stack_sort(t_stack *stack_a)
{
	int	i;

	i = 0;
	while (i < stack_a->actual_size - 1)
	{
		if (stack_a->numbers[i].value > stack_a->numbers[i + 1].value)
			return (0);
		i++;
	}
	return (1);
}

void	small_sort(t_stack *stack_a, t_stack *stack_b)
{
	if (is_stack_sort(stack_a))
		return ;
	if (stack_a->numbers[0].index > stack_a->numbers[1].index
		&& stack_a->numbers[0].index > stack_a->numbers[2].index)
		handle_rotate_up(stack_a, stack_b, "ra");
	else if (stack_a->numbers[1].index > stack_a->numbers[0].index
		&& stack_a->numbers[1].index > stack_a->numbers[2].index)
		handle_rotate_down(stack_a, stack_b, "rra");
	if (stack_a->numbers[0].index > stack_a->numbers[1].index)
		handle_swap(stack_a, stack_b, "sa");
}

void	init_op_count(t_count *ops)
{
	ops->sa = 0;
	ops->sb = 0;
	ops->ss = 0;
	ops->pa = 0;
	ops->pb = 0;
	ops->ra = 0;
	ops->rb = 0;
	ops->rr = 0;
	ops->rra = 0;
	ops->rrb = 0;
	ops->rrr = 0;
	ops->total_ops = 0;
	ops->flag = 0;
	ops->simple = 0;
	ops->medium = 0;
	ops->complex = 0;
	ops->adaptive = 0;
	ops->bench = 0;
	ops->selected_strat = 0;
}
