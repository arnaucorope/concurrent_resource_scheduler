/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 18:21:50 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/12 15:51:22 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongles(t_data *data)
{
	int	i;

	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!data->dongles)
		return (0);
	i = 0;
	while (i < data->number_of_coders)
	{
		data->dongles[i].id = i + 1;
		data->dongles[i].available = 1;
		data->dongles[i].cooldown_until = 0;
		data->dongles[i].waiters.items = NULL;
		data->dongles[i].waiters.size = 0;
		data->dongles[i].waiters.capacity = data->number_of_coders;
		data->dongles[i].waiters.items = malloc(sizeof(t_coder *)
				* data->number_of_coders);
		if (!data->dongles[i].waiters.items)
		{
			cleanup_dongles(data, i);
			return (0);
		}
		if (pthread_mutex_init(&data->dongles[i].mutex, NULL) != 0)
		{
			free(data->dongles[i].waiters.items);
			data->dongles[i].waiters.items = NULL;
			cleanup_dongles(data, i);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	init_coders(t_data *data)
{
	int	i;

	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coders)
		return (0);
	i = 0;
	while (i < data->number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].compiles_done = 0;
		data->coders[i].last_compile_start = 0;
		data->coders[i].right_dongle = &data->dongles[i];
		data->coders[i].left_dongle = &data->dongles[
			(i - 1 + data->number_of_coders) % data->number_of_coders];
		data->coders[i].data = data;
		data->coders[i].request.order = 0;
		data->coders[i].request.deadline = 0;
		if (pthread_mutex_init(&data->coders[i].state_mutex, NULL) != 0)
		{
			cleanup_coder_mutexes(data, i);
			free(data->coders);
			data->coders = NULL;
			return (0);
		}
		i++;
	}
	return (1);
}

static int	init_sync(t_data *data)
{
	if (pthread_mutex_init(&data->start_mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&data->start_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&data->start_mutex);
		return (0);
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		pthread_cond_destroy(&data->start_cond);
		pthread_mutex_destroy(&data->start_mutex);
		return (0);
	}
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		pthread_cond_destroy(&data->start_cond);
		pthread_mutex_destroy(&data->start_mutex);
		return (0);
	}
	if (pthread_mutex_init(&data->request_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->stop_mutex);
		pthread_mutex_destroy(&data->print_mutex);
		pthread_cond_destroy(&data->start_cond);
		pthread_mutex_destroy(&data->start_mutex);
		return (0);
	}
	return (1);
}

int	init_simulation(t_data *data)
{
	data->start_ready = 0;
	data->simulation_over = 0;
	data->request_counter = 0;
	if (!init_sync(data))
		return (0);
	if (!init_dongles(data))
	{
		cleanup_sync(data);
		return (0);
	}
	if (!init_coders(data))
	{
		cleanup_dongles(data, data->number_of_coders);
		cleanup_sync(data);
		return (0);
	}
	return (1);
}
