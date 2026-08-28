/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 14:22:57 by acoromin          #+#    #+#             */
/*   Updated: 2026/08/28 18:29:40 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdlib.h>

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
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_mutex_t	print_mutex;
};

struct s_dongle
{
	int				id;
	int				available;
	long			cooldown_until;
	pthread_mutex_t	mutex;
};

struct s_coder
{
	int				id;
	pthread_t		thread;
	int				compiles_done;
	long			last_compile_start;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_data			*data;
};

long	get_time_ms(void);
int		init_simulation(t_data *data);
void	cleanup_dongles(t_data *data, int count);
void	*coder_routine(void *arg);
int		run_simulation(t_data *data);
void	cleanup_sync(t_data *data);
void	cleanup_coders(t_data *data);
long	get_time_ms(void);

#endif
