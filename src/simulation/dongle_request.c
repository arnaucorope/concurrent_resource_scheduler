/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_request.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:21:40 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/14 12:27:22 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_request(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->request_mutex);
	coder->data->request_counter++;
	coder->request.order = coder->data->request_counter;
	pthread_mutex_unlock(&coder->data->request_mutex);
	pthread_mutex_lock(&coder->state_mutex);
	coder->request.deadline = coder->last_compile_start
		+ coder->data->time_to_burnout;
	pthread_mutex_unlock(&coder->state_mutex);
}

int	register_request(t_coder *coder)
{
	if (coder->left_dongle == coder->right_dongle)
	{
		pthread_mutex_lock(&coder->left_dongle->mutex);
		if (!heap_push(&coder->left_dongle->waiters, coder))
		{
			pthread_mutex_unlock(&coder->left_dongle->mutex);
			return (0);
		}
		pthread_mutex_unlock(&coder->left_dongle->mutex);
		return (1);
	}
	lock_dongles(coder);
	if (!heap_push(&coder->left_dongle->waiters, coder))
	{
		unlock_dongles(coder);
		return (0);
	}
	if (!heap_push(&coder->right_dongle->waiters, coder))
	{
		heap_remove(&coder->left_dongle->waiters, coder);
		unlock_dongles(coder);
		return (0);
	}
	unlock_dongles(coder);
	return (1);
}
