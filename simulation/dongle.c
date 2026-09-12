/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 12:19:13 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/12 15:08:04 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_dongles(t_coder *coder)
{
	if (is_simulation_over(coder->data))
		return (0);
	pthread_mutex_lock(&coder->data->request_mutex);
	coder->data->request_counter++;
	coder->request.order = coder->data->request_counter;
	pthread_mutex_unlock(&coder->data->request_mutex);
	pthread_mutex_lock(&coder->state_mutex);
	coder->request.deadline = coder->last_compile_start
		+ coder->data->time_to_burnout;
	pthread_mutex_unlock(&coder->state_mutex);
	return (1);
}
