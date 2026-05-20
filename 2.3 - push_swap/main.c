/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 11:05:26 by dbaise            #+#    #+#             */
/*   Updated: 2026/05/11 18:00:46 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//https://push-swap42-visualizer.vercel.app/
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
// retirer checker et tester

#include "push_swap.h"

int	fill_stack_2(t_stack *stack_a, char **tab)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (tab[j] != NULL)
	{
		if (is_number(tab[j]))
		{
			if (!is_valid_int(tab[j]))
			{
				free(stack_a->numbers);
				stack_a->numbers = NULL;
				return (1);
			}
			stack_a->numbers[i++].value = ft_atoi(tab[j]);
		}
		j++;
	}
	return (0);
}

int	fill_stack(char **tab, t_stack *stack_a)
{
	int	i;
	int	size;

	size = 0;
	i = 0;
	while (tab[i] != NULL)
	{
		if (is_number(tab[i]))
			size++;
		i++;
	}
	if (size == 0)
		return (1);
	stack_a->numbers = malloc(sizeof(t_num) * size);
	if (!stack_a->numbers)
		return (1);
	stack_a->actual_size = size;
	if (fill_stack_2(stack_a, tab))
		return (1);
	return (0);
}

char	**get_tab(int argc, char **argv, int *free_flag)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (argv[i][0] != '-')
		{
			if (ft_strchr(argv[i], ' '))
			{
				*free_flag = 1;
				return (ft_split(argv[i], ' '));
			}
			return (&argv[i]);
		}
		i++;
	}
	return (NULL);
}

int	parser(int argc, char *argv[], t_stack *stack_a)
{
	char	**tab;
	int		free_flag;

	free_flag = 0;
	tab = get_tab(argc, argv, &free_flag);
	if (!tab)
		return (1);
	if (fill_stack(tab, stack_a) || index_numbers(stack_a))
	{
		if (free_flag)
			free_tab(tab);
		return (1);
	}
	if (free_flag)
		free_tab(tab);
	if (!stack_a->numbers || check_duplicate(stack_a))
		return (1);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_stack	stack_a;
	t_stack	stack_b;

	stack_a.actual_size = 0;
	stack_a.numbers = NULL;
	init_op_count(&stack_a.ops_count);
	if (argc == 1)
		return (0);
	if (argc < 2 || validate_args(argc, argv, &stack_a.ops_count)
		|| parser(argc, argv, &stack_a))
		return (free_and_display_error(&stack_a));
	stack_a.chaos = compute_disorder(&stack_a);
	stack_b.actual_size = 0;
	stack_b.numbers = malloc(sizeof(t_num) * stack_a.actual_size);
	if (!stack_b.numbers)
		return (free_and_display_error(&stack_a));
	strat_selector(&stack_a, &stack_b);
	if (stack_a.ops_count.bench == 1)
		display_all(&stack_a);
	free(stack_a.numbers);
	free(stack_b.numbers);
	return (0);
}
