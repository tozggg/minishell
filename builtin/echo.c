/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 16:41:09 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/19 20:34:02 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../libft/libft.h"

int	do_echo(int ac, char **av)
{
	int	nonl;

	nonl = 0;
	if (ac > 1 && ft_strequ(av[1], "-n"))
	{
		av++;
		nonl = 1;
	}
	while (*(++av))
	{
		ft_putstr_fd(*av, STDOUT_FILENO);
		if (av[1] != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (!nonl)
		ft_putendl_fd("", STDOUT_FILENO);
	return (0);
}
