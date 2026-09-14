/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 16:57:21 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/14 12:27:15 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->start_mutex);
	while (coder->data->start_ready == 0)
		pthread_cond_wait(&coder->data->start_cond,
			&coder->data->start_mutex);
	if (coder->data->start_ready == -1)
	{
		pthread_mutex_unlock(&coder->data->start_mutex);
		return (0);
	}
	pthread_mutex_unlock(&coder->data->start_mutex);
	return (1);
}

void	print_status(t_coder *coder, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&coder->data->print_mutex);
	if (is_simulation_over(coder->data))
	{
		pthread_mutex_unlock(&coder->data->print_mutex);
		return ;
	}
	timestamp = get_time_ms() - coder->data->start_time;
	printf("%ld %d %s\n", timestamp, coder->id, status);
	pthread_mutex_unlock(&coder->data->print_mutex);
}

static int	coder_cycle(t_coder *coder)
{
	if (!take_dongles(coder))
		return (0);
	print_status(coder, "has taken a dongle");
	print_status(coder, "has taken a dongle");
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->state_mutex);
	print_status(coder, "is compiling");
	if (!sleep_simulation(coder->data, coder->data->time_to_compile))
		return (0);
	pthread_mutex_lock(&coder->state_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->state_mutex);
	release_dongles(coder);
	print_status(coder, "is debugging");
	if (!sleep_simulation(coder->data, coder->data->time_to_debug))
		return (0);
	print_status(coder, "is refactoring");
	if (!sleep_simulation(coder->data, coder->data->time_to_refactor))
		return (0);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (!wait_for_start(coder))
		return (NULL);
	while (!is_simulation_over(coder->data))
	{
		if (!coder_cycle(coder))
			break ;
	}
	return (NULL);
}
