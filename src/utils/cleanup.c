/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 16:07:10 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/14 11:08:14 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup_dongles(t_data *data, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		free(data->dongles[i].waiters.items);
		data->dongles[i].waiters.items = NULL;
		i++;
	}
	free(data->dongles);
	data->dongles = NULL;
}

static void	cleanup_coder_mutexes(t_data *data, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&data->coders[i].state_mutex);
		i++;
	}
}

void	cleanup_coders(t_data *data, int count)
{
	cleanup_coder_mutexes(data, count);
	free(data->coders);
	data->coders = NULL;
}

void	cleanup_sync(t_data *data)
{
	pthread_mutex_destroy(&data->print_mutex);
	pthread_cond_destroy(&data->start_cond);
	pthread_mutex_destroy(&data->start_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	pthread_mutex_destroy(&data->request_mutex);
}

void	cleanup_simulation(t_data *data)
{
	cleanup_coders(data, data->number_of_coders);
	cleanup_dongles(data, data->number_of_coders);
	cleanup_sync(data);
}
