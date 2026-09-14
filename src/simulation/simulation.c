/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 17:01:02 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/14 12:18:05 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	join_created_coders(t_data *data, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
}

int	create_coder_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if (pthread_create(&data->coders[i].thread, NULL,
				coder_routine, &data->coders[i]) != 0)
		{
			pthread_mutex_lock(&data->start_mutex);
			data->start_ready = -1;
			pthread_cond_broadcast(&data->start_cond);
			pthread_mutex_unlock(&data->start_mutex);
			join_created_coders(data, i);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	start_threads(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->start_mutex);
	data->start_time = get_time_ms();
	i = 0;
	while (i < data->number_of_coders)
	{
		data->coders[i].last_compile_start = data->start_time;
		i++;
	}
	data->start_ready = 1;
	pthread_cond_broadcast(&data->start_cond);
	pthread_mutex_unlock(&data->start_mutex);
}

int	run_simulation(t_data *data)
{
	pthread_t	referee;

	if (!create_coder_threads(data))
		return (0);
	if (pthread_create(&referee, NULL, referee_routine, data) != 0)
	{
		pthread_mutex_lock(&data->start_mutex);
		data->start_ready = -1;
		pthread_cond_broadcast(&data->start_cond);
		pthread_mutex_unlock(&data->start_mutex);
		join_created_coders(data, data->number_of_coders);
		return (0);
	}
	start_threads(data);
	join_created_coders(data, data->number_of_coders);
	pthread_join(referee, NULL);
	return (1);
}
