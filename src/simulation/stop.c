/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 17:23:42 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/10 17:37:42 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_simulation_over(t_data *data)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->simulation_over = 1;
	pthread_mutex_unlock(&data->stop_mutex);
}

int	is_simulation_over(t_data *data)
{
	int	value;

	pthread_mutex_lock(&data->stop_mutex);
	value = data->simulation_over;
	pthread_mutex_unlock(&data->stop_mutex);
	return (value);
}
