/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 16:41:09 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/18 16:54:43 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../libft/libft.h"

int	do_echo(char **av)
{
	int	nonl;

	nonl = 0;
	if (av[1] && ft_strequ(av[1], "-n"))
	{
		av++;
		nonl = 1;
	}
	while (*(++av))
	{
		printf("%s", *av);
		if (av[1] != NULL)
			printf(" ");
	}
	if (!nonl)
		printf("\n");
	return (0);
}
