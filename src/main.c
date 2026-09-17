/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 15:26:05 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/17 12:56:03 by acoromin         ###   ########.fr       */
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
	if (data.number_of_compiles_required == 0)
	{
		cleanup_simulation(&data);
		return (0);
	}
	if (!run_simulation(&data))
	{
		cleanup_simulation(&data);
		return (1);
	}
	cleanup_simulation(&data);
	return (0);
}
