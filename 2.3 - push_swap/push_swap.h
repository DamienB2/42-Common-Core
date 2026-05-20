/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 11:12:49 by dbaise            #+#    #+#             */
/*   Updated: 2026/05/14 10:26:13 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdlib.h>
# include "libft/libft.h"

typedef struct s_num
{
	int		value;
	int		index;
}	t_num;

typedef struct s_count
{
	int	sa;
	int	sb;
	int	ss;
	int	pa;
	int	pb;
	int	ra;
	int	rb;
	int	rr;
	int	rra;
	int	rrb;
	int	rrr;
	int	total_ops;
	int	flag;
	int	simple;
	int	medium;
	int	complex;
	int	adaptive;
	int	bench;
	int	selected_strat;

}	t_count;

typedef struct stack
{
	t_num	*numbers;
	int		actual_size;
	t_count	ops_count;
	double	chaos;
}	t_stack;

// utils.c
double	compute_disorder(t_stack *stack_a);
int		is_number(char *s);
int		check_duplicate(struct stack *stack_a);
int		free_and_display_error(t_stack *stack_a);
void	free_tab(char **tab);

//utils2.c
int		ft_sqrt(int nb);
int		is_valid_int(char *str);
int		is_stack_sort(t_stack *stack_a);
void	init_op_count(t_count *ops);

// utils_operations.c
void	swap(t_stack *stack);
void	push(t_stack *dest, t_stack *src);
void	rotate_up(t_stack *stack);
void	rotate_down(t_stack *stack);

// utils_op_handler.c
void	handle_swap(t_stack *stack_a, t_stack *stack_b, char *s);
void	handle_push(t_stack *stack_a, t_stack *stack_b, char *s);
void	handle_rotate_up(t_stack *stack_a, t_stack *stack_b, char *s);
void	handle_rotate_down(t_stack *stack_a, t_stack *stack_b, char *s);

//insertion_sort.c
void	insertion_sort(t_stack *stack_a, t_stack *stack_b);
int		find_direction(t_stack *stack_a, int index_to_find);

//chunck_sort.c
void	chunk_sort(t_stack *stack_a, t_stack *stack_b);

//radix_sort.c
void	radix_sort(t_stack *stack_a, t_stack *stack_b);

// flag_parser.c
int		flag_parser(char *str, t_count *ops_count);
int		is_number(char *s);
int		validate_args(int argc, char **argv, t_count *ops_count);

// index
int		index_numbers(t_stack *stack);
void	index_numbers_2(t_stack *stack, int *copy);

//strat_selector.c
void	strat_selector(t_stack *stack_a, t_stack *stack_b);

//bench.c
void	display_all(t_stack *stack_a);

//mini
void	mini_sort3(t_stack *stack_a, t_stack *stack_b);
#endif
