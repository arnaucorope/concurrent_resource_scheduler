/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 15:26:05 by acoromin          #+#    #+#             */
/*   Updated: 2026/08/27 16:33:48 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "parse.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_parse_error	error;
	t_data			data;

	error = parse_args(argc, argv);
	if (error != PARSE_OK)
	{
		print_parse_error(error);
		return (0);
	}
	convert_arguments(argv, &data);
	if (!init_simulation(data))
		return (1);
	return (0);
}
