/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 16:57:21 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/12 12:17:15 by acoromin         ###   ########.fr       */
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

void *coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (!wait_for_start(coder))
		return (NULL);
	while (!is_simulation_over(coder->data))
	{

	}
	return (NULL);
}
