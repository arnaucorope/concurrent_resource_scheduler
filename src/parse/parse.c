/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 18:34:14 by acoromin          #+#    #+#             */
/*   Updated: 2026/08/24 14:56:47 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "codexion.h"

static int	validate_number(char *arg, int i)
{
	long	value;

	value = ft_atol(arg);
	if (value == -1)
		return (0);
	if (i == 1 && value == 0)
		return (0);
	return (1);
}

static int	validate_scheduler(char *str)
{
	if (ft_strcmp(str, "fifo") != 0 && ft_strcmp(str, "edf") != 0)
		return (0);
	return (1);
}

t_parse_error	parse_args(int argc, char **argv)
{
	int	i;

	if (argc != 9)
		return (ERR_ARGC);
	i = 1;
	while (i < 8)
	{
		if (!validate_number(argv[i], i))
			return ((t_parse_error)i);
		i++;
	}
	if (!validate_scheduler(argv[8]))
		return (ERR_SCHEDULER);
	return (PARSE_OK);
}

void	convert_arguments(char **argv, t_data *data)
{
	data->number_of_coders = ft_atol(argv[1]);
	data->time_to_burnout = ft_atol(argv[2]);
	data->time_to_compile = ft_atol(argv[3]);
	data->time_to_debug = ft_atol(argv[4]);
	data->time_to_refactor = ft_atol(argv[5]);
	data->number_of_compiles_required = ft_atol(argv[6]);
	data->dongle_cooldown = ft_atol(argv[7]);
	if (ft_strcmp(argv[8], "fifo") == 0)
		data->scheduler = SCHEDULER_FIFO;
	else
		data->scheduler = SCHEDULER_EDF;
}
