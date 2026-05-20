/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:50:30 by dmonseur          #+#    #+#             */
/*   Updated: 2026/05/20 11:40:55 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	display_disorder(t_stack *stack_a)
{
	double	rest;
	int		i;

	i = 4;
	if (stack_a->chaos == 1)
	{
		ft_putstr_fd("[bench] disorder: 100%\n", 2);
		return ;
	}
	rest = stack_a->chaos;
	ft_putstr_fd("[bench] disorder: ", 2);
	while (i--)
	{
		if (i == 1)
			ft_putchar_fd('.', 2);
		rest *= 10;
		ft_putnbr_fd((int) rest % 10, 2);
		rest -= (int) rest;
	}
	ft_putstr_fd("%\n", 2);
}

void	display_strat(t_stack *stack_a)
{
	if (stack_a->ops_count.adaptive == 1)
	{
		if (stack_a->ops_count.simple == 1)
			ft_putstr_fd("[bench] strategy: Adaptive / O(n2)\n", 2);
		if (stack_a->ops_count.medium == 1)
			ft_putstr_fd("[bench] strategy: Adaptive / O(n√n)\n", 2);
		if (stack_a->ops_count.complex == 1)
			ft_putstr_fd("[bench] strategy: Adaptive / n log n\n", 2);
	}
	else if (stack_a->ops_count.simple == 1)
		ft_putstr_fd("[bench] strategy: Simple / O(n2)\n", 2);
	else if (stack_a->ops_count.medium == 1)
		ft_putstr_fd("[bench] strategy: Medium / O(n√n)\n", 2);
	else if (stack_a->ops_count.complex == 1)
		ft_putstr_fd("[bench] strategy: Complex / n log n\n", 2);
}

void	display_operations(t_stack *stack_a)
{
	ft_putstr_fd("[bench] total_ops: ", 2);
	ft_putnbr_fd(stack_a->ops_count.total_ops, 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("[bench] sa: ", 2);
	ft_putnbr_fd(stack_a->ops_count.sa, 2);
	ft_putstr_fd(" sb: ", 2);
	ft_putnbr_fd(stack_a->ops_count.sb, 2);
	ft_putstr_fd(" ss: ", 2);
	ft_putnbr_fd(stack_a->ops_count.ss, 2);
	ft_putstr_fd(" pa: ", 2);
	ft_putnbr_fd(stack_a->ops_count.pa, 2);
	ft_putstr_fd(" pb: ", 2);
	ft_putnbr_fd(stack_a->ops_count.pb, 2);
	ft_putchar_fd('\n', 2);
}

void	display_operations2(t_stack *stack_a)
{
	ft_putstr_fd("[bench] ra: ", 2);
	ft_putnbr_fd(stack_a->ops_count.ra, 2);
	ft_putstr_fd(" rb: ", 2);
	ft_putnbr_fd(stack_a->ops_count.rb, 2);
	ft_putstr_fd(" rr: ", 2);
	ft_putnbr_fd(stack_a->ops_count.rr, 2);
	ft_putstr_fd(" rra: ", 2);
	ft_putnbr_fd(stack_a->ops_count.rra, 2);
	ft_putstr_fd(" rrb: ", 2);
	ft_putnbr_fd(stack_a->ops_count.rrb, 2);
	ft_putstr_fd(" rrr: ", 2);
	ft_putnbr_fd(stack_a->ops_count.rrr, 2);
	ft_putchar_fd('\n', 2);
}

void	display_all(t_stack *stack_a)
{
	display_disorder(stack_a);
	display_strat(stack_a);
	display_operations(stack_a);
	display_operations2(stack_a);
}
