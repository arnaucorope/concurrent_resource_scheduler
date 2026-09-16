/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 15:26:05 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/16 15:20:00 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "parse.h"

int	main(int argc, char **argv)
{
	t_parse_error	error;
	t_data			data;

	error = parse_args(argc, argv);
	if (error != PARSE_OK)
	{
		print_parse_error(error);
		return (1);
	}
	convert_arguments(argv, &data);
	if (!init_simulation(&data))
		return (1);
	if (!run_simulation(&data))
	{
		cleanup_simulation(&data);
		return (1);
	}
	cleanup_simulation(&data);
	return (0);
}
