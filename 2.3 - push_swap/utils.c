/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 11:09:44 by dmonseur          #+#    #+#             */
/*   Updated: 2026/05/11 16:22:09 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

double	compute_disorder(t_stack *stack_a)
{
	int	mistakes;
	int	total_pairs;
	int	i;
	int	j;

	mistakes = 0;
	total_pairs = 0;
	i = 0;
	if (stack_a->actual_size == 0 || stack_a->actual_size == 1)
		return (0);
	while (i < stack_a->actual_size - 1)
	{
		j = i + 1;
		while (j < stack_a->actual_size)
		{
			total_pairs++;
			if (stack_a->numbers[i].value > stack_a->numbers[j].value)
				mistakes++;
			j++;
		}
		i++;
	}
	return ((double) mistakes / total_pairs);
}

int	is_number(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	check_duplicate(struct stack *stack_a)
{
	int	i;
	int	j;

	i = 0;
	while (i < stack_a->actual_size - 1)
	{
		j = i + 1;
		while (j < stack_a->actual_size)
		{
			if (stack_a->numbers[i].value == stack_a->numbers[j].value)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	free_and_display_error(t_stack *stack_a)
{
	free(stack_a->numbers);
	write(2, "Error\n", 6);
	return (1);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}
