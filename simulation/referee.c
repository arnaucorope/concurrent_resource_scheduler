/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   referee.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 17:44:13 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/12 11:58:31 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	wait_referee_start(t_data *data)
{
	pthread_mutex_lock(&data->start_mutex);
	while (data->start_ready == 0)
		pthread_cond_wait(&data->start_cond,
			&data->start_mutex);
	if (data->start_ready == -1)
	{
		pthread_mutex_unlock(&data->start_mutex);
		return (0);
	}
	pthread_mutex_unlock(&data->start_mutex);
	return (1);
}

int	all_compiles_done(t_data *data)
{
	int	i;
	int	compiles;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].state_mutex);
		compiles = data->coders[i].compiles_done;
		pthread_mutex_unlock(&data->coders[i].state_mutex);
		if (compiles < data->number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

static void	print_burnout(t_data *data, int i)
{
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d burned out\n",
		get_time_ms() - data->start_time,
		data->coders[i].id);
	pthread_mutex_unlock(&data->print_mutex);
}

int	check_burnout(t_data *data)
{
	int		i;
	long	last_compile;
	long	now;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].state_mutex);
		last_compile = data->coders[i].last_compile_start;
		now = get_time_ms();
		pthread_mutex_unlock(&data->coders[i].state_mutex);
		if (now - last_compile >= data->time_to_burnout)
		{
			set_simulation_over(data);
			print_burnout(data, i);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*referee_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	if (!wait_referee_start(data))
		return (NULL);
	while (!is_simulation_over(data))
	{
		if (check_burnout(data))
			break ;
		if (all_compiles_done(data))
		{
			set_simulation_over(data);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
