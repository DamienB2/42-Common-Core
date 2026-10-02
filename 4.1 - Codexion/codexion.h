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

typedef struct s_queue_node
{
	int		id;
	long	seq;
	long	deadline;
}	t_queue_node;

typedef struct s_queue
{
	t_queue_node	nodes[2];
	int				size;
	int				scheduler;
}	t_queue;

typedef struct s_dongle
{
	int				taken;
	long			free_at;
	long			next_seq;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	t_queue			queue;
}	t_dongle;

typedef struct s_sim
{
	pthread_mutex_t	print_lock;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	counters_lock;
	pthread_t		monitor_thread;
	long			start_time;
	int				scheduler;
	int				nb_coders;
	int				nb_dongles;
	int				nb_compiles_required;
	int				burnout_t;
	int				dongle_cd;
	int				stop;
	t_dongle		*dongles;
	long			*last_compile_start;
	int				*nb_compiles_done;
}	t_sim;

typedef struct s_coder
{
	int			id;
	int			compile_t;
	int			debug_t;
	int			refactor_t;
	int			burnout_t;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	pthread_t	thread;
	t_sim		*sim;
}	t_coder;

// parser.c
int				parser(int argc, char *argv[], t_params	*params);

// parser_helper.c
int				ft_isdigit(int c);
int				ft_is_str_digit(char *str);
int				ft_atoi(const char *nptr);

// time_helper.c
long			get_time_ms(void);
long			get_sim_time(long simulation_time);

// coder.c
void			*coder_routine(void *arg);

// coder_actions.c
void			coder_compile(t_coder *coder);
void			coder_debug(t_coder *coder);
void			coder_refactor(t_coder *coder);
int				finish_compile(t_coder *coder);

// simulation.c
int				simulation(t_params params);
int				start_coders(t_coder *coders, int nb_coders, t_sim *sim);
void			assign_dongles(t_coder *coder, t_sim *sim, int index);

// sim_init.c
int				init_simulation(t_params params, t_sim *sim);
void			init_counters(t_sim *sim);
int				init_dongles(t_sim *sim);
void			init_coders(t_coder *coders, t_params params, t_sim *sim);

// sim_utils.c
int				sim_should_stop(t_sim *sim);
void			sim_stop_simulation(t_sim *sim);
void			cleanup_simulation(t_sim *sim, t_coder *coders);
void			print_state(t_coder *coder, char *state);
void			print_death(t_sim *sim, int id);

// sim_utils_2.c
void			set_last_compile_start(t_sim *sim, int index, long time);
long			get_last_compile_start(t_sim *sim, int index);
void			increment_compiles_done(t_sim *sim, int index);
int				all_compiles_done(t_sim *sim);

// monitor.c
void			*monitor_routine(void *arg);

// priority_queue.c
void			pq_push(t_queue *pq, t_queue_node node);
t_queue_node	pq_pop(t_queue *pq);
int				pq_peek_id(t_queue *pq);
void			pq_remove_id(t_queue *pq, int id);

// dongle.c
int				take_dongle(t_dongle *dongle,
					int id, long deadline, t_sim *sim);
void			release_dongle(t_dongle *dongle, int cooldown);
void			wake_all_dongles(t_sim *sim);

// dongle_init.c
void			init_dongle(t_dongle *dongle, int scheduler);
void			destroy_dongles(t_sim *sim);
void			destroy_dongle(t_dongle *dongle);

#endif
