/* ************************************************************************** */
/*		                                                                      */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 14:47:02 by dbaise            #+#    #+#             */
/*   Updated: 2026/08/25 15:02:25 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_params
{
	int		nb_coders;		//number_of_coders
	int		burnout_t;		//time_to_burnout
	int		compile_t;		//time_to_compile
	int		debug_t;		//time_to_debug
	int		refactor_t;		//time_to_refactor
	int		nb_compiles;	//number_of_compiles_required
	int		dongle_cd;		//dongle_cooldown
	int		scheduler;		//scheduler 0 is fifo and 1 is edf
}	t_params;

typedef struct s_request
{
	int		active;
	int		id;
	long	seq;
	long	deadline;
}	t_request;
 
typedef struct s_sim
{
	pthread_mutex_t	print_lock;
	pthread_cond_t	turn_cond;
	int				nb_say;
	long			start_time;
	int				scheduler;
	int				nb_coders;
	long			next_seq;
	t_request		*waiting;
}	t_sim;
 
typedef struct s_coder
{
	int			id;
	int			nb_compiles_required;
	int			compile_t;
	int			debug_t;
	int			refactor_t;
	int			burnout_t;
	pthread_t	thread;
	t_sim		*sim;
}	t_coder;

typedef struct s_dongle
{
	int				id;
	int				cooldown;
	pthread_mutex_t	dongle_lock;

}	t_dongle;

//	parser.c
int		parser(int argc, char *argv[], t_params	*params);

//	parser_helper.c
int		ft_isdigit(int c);
int		ft_is_str_digit(char *str);
int		ft_atoi(const char *nptr);

//time_helper.c
long	get_time_ms(void);
long	get_sim_time(long simulation_time);

//codexion.c
void	*say(void *arg);

//simulation.c
int		init_simulation(t_params params, t_sim *sim);
int		simulation(t_params params);

#endif
