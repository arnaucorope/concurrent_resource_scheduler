/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 15:46:37 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/13 19:45:31 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

int	sleep_simulation(t_data *data, long duration_ms)
{
	long	start;

	start = get_time_ms();
	while (!is_simulation_over(data))
	{
		if (get_time_ms() - start >= duration_ms)
			return (1);
		usleep(1000);
	}
	return (0);
}
