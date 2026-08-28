/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 16:07:10 by acoromin          #+#    #+#             */
/*   Updated: 2026/08/28 18:21:58 by acoromin         ###   ########.fr       */
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
		i++;
	}
	free(data->dongles);
	data->dongles = NULL;
}

void	cleanup_coders(t_data *data)
{
	free(data->coders);
	data->coders = NULL;
}

void	cleanup_sync(t_data *data)
{
	pthread_mutex_destroy(&data->print_mutex);
	pthread_cond_destroy(&data->start_cond);
	pthread_mutex_destroy(&data->start_mutex);
}
