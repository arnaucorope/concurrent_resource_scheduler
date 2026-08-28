/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 20:59:40 by acoromin          #+#    #+#             */
/*   Updated: 2026/08/24 13:21:09 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

void	print_parse_error(t_parse_error error)
{
	if (error == ERR_ARGC)
		ft_putstr_fd("Error: wrong number of arguments\n", 2);
	else if (error == ERR_CODERS)
		ft_putstr_fd("Error: invalid number of coders\n", 2);
	else if (error == ERR_BURNOUT)
		ft_putstr_fd("Error: invalid time to burnout\n", 2);
	else if (error == ERR_COMPILE)
		ft_putstr_fd("Error: invalid time to compile\n", 2);
	else if (error == ERR_DEBUG)
		ft_putstr_fd("Error: invalid time to debug\n", 2);
	else if (error == ERR_REFACTOR)
		ft_putstr_fd("Error: invalid time to refactor\n", 2);
	else if (error == ERR_REQUIRED)
		ft_putstr_fd("Error: invalid number of required compiles\n", 2);
	else if (error == ERR_COOLDOWN)
		ft_putstr_fd("Error: invalid dongle cooldown\n", 2);
	else if (error == ERR_SCHEDULER)
		ft_putstr_fd("Error: scheduler must be fifo or edf\n", 2);
}
