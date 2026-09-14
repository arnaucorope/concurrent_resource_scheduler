/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 17:31:36 by acoromin          #+#    #+#             */
/*   Updated: 2026/08/27 16:35:54 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

long	ft_atol(char *s)
{
	long	n;
	int		digit;

	if (!*s)
		return (-1);
	if (*s == '+')
		s++;
	if (!*s)
		return (-1);
	n = 0;
	while (*s >= '0' && *s <= '9')
	{
		digit = *s - '0';
		if (n > (INT_MAX - digit) / 10)
			return (-1);
		n = n * 10 + digit;
		s++;
	}
	if (*s)
		return (-1);
	return (n);
}

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}
