/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_op_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:11:20 by dmonseur          #+#    #+#             */
/*   Updated: 2026/05/12 14:15:27 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	handle_swap(t_stack *stack_a, t_stack *stack_b, char *s)
{
	stack_a->ops_count.total_ops++;
	if (ft_strncmp(s, "sa", 3) == 0)
	{
		swap(stack_a);
		stack_a->ops_count.sa++;
	}
	else if (ft_strncmp(s, "sb", 3) == 0)
	{
		swap(stack_b);
		stack_a->ops_count.sb++;
	}
	else if (ft_strncmp(s, "ss", 3) == 0)
	{
		swap(stack_a);
		swap(stack_b);
		stack_a->ops_count.ss++;
	}
	ft_putstr_fd(s, 1);
	ft_putchar_fd('\n', 1);
}

void	handle_push(t_stack *stack_a, t_stack *stack_b, char *s)
{
	stack_a->ops_count.total_ops++;
	if (ft_strncmp(s, "pa", 3) == 0)
	{
		push(stack_a, stack_b);
		stack_a->ops_count.pa++;
	}
	else if (ft_strncmp(s, "pb", 3) == 0)
	{
		push(stack_b, stack_a);
		stack_a->ops_count.pb++;
	}
	ft_putstr_fd(s, 1);
	ft_putchar_fd('\n', 1);
}

void	handle_rotate_up(t_stack *stack_a, t_stack *stack_b, char *s)
{
	stack_a->ops_count.total_ops++;
	if (ft_strncmp(s, "ra", 3) == 0)
	{
		rotate_up(stack_a);
		stack_a->ops_count.ra++;
	}
	else if (ft_strncmp(s, "rb", 3) == 0)
	{
		rotate_up(stack_b);
		stack_a->ops_count.rb++;
	}
	else if (ft_strncmp(s, "rr", 3) == 0)
	{
		rotate_up(stack_a);
		rotate_up(stack_b);
		stack_a->ops_count.rr++;
	}
	ft_putstr_fd(s, 1);
	ft_putchar_fd('\n', 1);
}

void	handle_rotate_down(t_stack *stack_a, t_stack *stack_b, char *s)
{
	stack_a->ops_count.total_ops++;
	if (ft_strncmp(s, "rra", 3) == 0)
	{
		rotate_down(stack_a);
		stack_a->ops_count.rra++;
	}
	else if (ft_strncmp(s, "rrb", 3) == 0)
	{
		rotate_down(stack_b);
		stack_a->ops_count.rrb++;
	}
	else if (ft_strncmp(s, "rrr", 3) == 0)
	{
		rotate_down(stack_a);
		rotate_down(stack_b);
		stack_a->ops_count.rrr++;
	}
	ft_putstr_fd(s, 1);
	ft_putchar_fd('\n', 1);
}
