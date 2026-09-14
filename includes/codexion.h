/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 14:22:57 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/14 12:23:48 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_data		t_data;
typedef struct s_dongle		t_dongle;
typedef struct s_coder		t_coder;

typedef enum e_scheduler
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}	t_scheduler;

struct s_data
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	t_scheduler		scheduler;
	int				start_ready;
	long			start_time;
	pthread_mutex_t	start_mutex;
	pthread_cond_t	start_cond;
	int				simulation_over;
	pthread_mutex_t	stop_mutex;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_mutex_t	print_mutex;
	int				request_counter;
	pthread_mutex_t	request_mutex;
};

typedef struct s_request
{
	long	order;
	long	deadline;
}	t_request;

typedef struct s_heap
{
	t_coder	**items;
	int		size;
	int		capacity;
}	t_heap;

struct s_dongle
{
	int				id;
	int				available;
	long			cooldown_until;
	pthread_mutex_t	mutex;
	t_heap			waiters;
};

struct s_coder
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	state_mutex;
	int				compiles_done;
	long			last_compile_start;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_data			*data;
	t_request		request;
};

int		init_simulation(t_data *data);
void	cleanup_dongles(t_data *data, int count);
void	*coder_routine(void *arg);
int		run_simulation(t_data *data);
void	cleanup_sync(t_data *data);
void	cleanup_coders(t_data *data, int count);
long	get_time_ms(void);
void	set_simulation_over(t_data *data);
int		is_simulation_over(t_data *data);
void	*referee_routine(void *arg);
int		take_dongles(t_coder *coder);
int		heap_push(t_heap *heap, t_coder *coder);
void	heap_pop(t_heap *heap);
void	lock_dongles(t_coder *coder);
void	unlock_dongles(t_coder *coder);
void	print_status(t_coder *coder, char *status);
t_coder	*heap_peek(t_heap *heap);
void	heap_remove(t_heap *heap, t_coder *coder);
void	release_dongles(t_coder *coder);
int		sleep_simulation(t_data *data, long duration_ms);
void	cleanup_simulation(t_data *data);
void	init_request(t_coder *coder);
int		register_request(t_coder *coder);

#endif
