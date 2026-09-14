/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 12:19:13 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/14 12:24:08 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	can_take_dongles(t_coder *coder)
{
	long	now;

	now = get_time_ms();
	if (heap_peek(&coder->left_dongle->waiters) != coder)
		return (0);
	if (heap_peek(&coder->right_dongle->waiters) != coder)
		return (0);
	if (!coder->left_dongle->available
		|| !coder->right_dongle->available)
		return (0);
	if (coder->left_dongle->cooldown_until > now
		|| coder->right_dongle->cooldown_until > now)
		return (0);
	return (1);
}

static int	try_take_dongles(t_coder *coder)
{
	if (coder->left_dongle == coder->right_dongle)
		return (0);
	lock_dongles(coder);
	if (!can_take_dongles(coder))
	{
		unlock_dongles(coder);
		return (0);
	}
	coder->left_dongle->available = 0;
	coder->right_dongle->available = 0;
	heap_pop(&coder->left_dongle->waiters);
	heap_pop(&coder->right_dongle->waiters);
	unlock_dongles(coder);
	return (1);
}

int	take_dongles(t_coder *coder)
{
	if (is_simulation_over(coder->data))
		return (0);
	init_request(coder);
	if (!register_request(coder))
		return (0);
	while (!is_simulation_over(coder->data))
	{
		if (try_take_dongles(coder))
			return (1);
		usleep(1000);
	}
	return (0);
}

void	release_dongles(t_coder *coder)
{
	long	now;

	if (coder->left_dongle == coder->right_dongle)
		return ;
	lock_dongles(coder);
	now = get_time_ms();
	coder->left_dongle->available = 1;
	coder->right_dongle->available = 1;
	coder->left_dongle->cooldown_until
		= now + coder->data->dongle_cooldown;
	coder->right_dongle->cooldown_until
		= now + coder->data->dongle_cooldown;
	unlock_dongles(coder);
}
