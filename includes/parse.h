/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 20:27:19 by acoromin          #+#    #+#             */
/*   Updated: 2026/08/24 14:59:24 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "codexion.h"
# include <limits.h>
# include <unistd.h>

typedef enum e_parse_error
{
	PARSE_OK,
	ERR_CODERS,
	ERR_BURNOUT,
	ERR_COMPILE,
	ERR_DEBUG,
	ERR_REFACTOR,
	ERR_REQUIRED,
	ERR_COOLDOWN,
	ERR_SCHEDULER,
	ERR_ARGC
}	t_parse_error;

long			ft_atol(char *s);
int				ft_strcmp(char *s1, char *s2);
void			ft_putstr_fd(char *str, int fd);

t_parse_error	parse_args(int argc, char **argv);
void			print_parse_error(t_parse_error error);
void			convert_arguments(char **argv, t_data *data);

#endif
