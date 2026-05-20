/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index_finder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 14:13:08 by dmonseur          #+#    #+#             */
/*   Updated: 2026/05/19 11:49:57 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

// Index numbers
void	sort_tab(int *tab, int size)
{
	int	i;
	int	j;
	int	tmp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (tab[j] > tab[j + 1])
			{
				tmp = tab[j];
				tab[j] = tab[j + 1];
				tab[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	index_numbers_2(t_stack *stack, int *copy)
{
	int	i;
	int	j;

	i = 0;
	while (i < stack->actual_size)
	{
		j = 0;
		while (j < stack->actual_size)
		{
			if (copy[i] == stack->numbers[j].value)
			{
				stack->numbers[j].index = i;
				break ;
			}
			j++;
		}
		i++;
	}
	free(copy);
}

int	index_numbers(t_stack *stack)
{
	int		i;
	int		*copy;

	i = 0;
	copy = malloc(sizeof(int) * stack->actual_size);
	if (!copy)
		return (1);
	while (i < stack->actual_size)
	{
		copy[i] = stack->numbers[i].value;
		i++;
	}
	sort_tab(copy, stack->actual_size);
	index_numbers_2(stack, copy);
	return (0);
}
