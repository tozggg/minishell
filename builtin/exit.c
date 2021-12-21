/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 23:01:42 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/21 15:30:12 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../libft/libft.h"

static long	ft_atol(const char *nptr, int *err)
{
	int			sign;
	long long	result;

	*err = 0;
	sign = 1;
	if (*nptr == '-')
		sign = -1;
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	result = 0;
	while (*err != 1 && ft_isdigit(*nptr))
	{
		result = result * 10 + (*nptr++ - '0');
		if (sign > 0 && result >= LONG_MAX / 10 && *nptr > LONG_MAX % 10 + '0')
			*err = 1;
		if (sign < 0 && result >= LONG_MAX / 10 && *nptr > LONG_MAX % 10 + '1')
			*err = 1;
	}
	if (*err || *nptr != '\0')
	{
		*err = 1;
		return (255);
	}
	return (sign * result);
}

/* test cases
 * exit		--								exit,		ret = 0
 * exit 3   -- 								exit,		ret = 3
 * exit 256 --								exit,		ret = 0
 * exit 3 a -- too many arguments,			no exit,	ret = 1
 * exit a 3 -- numeric argument required,	exit,		ret = 255
 * exit a   -- numeric argument required,	exit,		ret = 255
 * exit LMAX+1 numeric argument required,	exit,		ret = 255
*/
int	do_exit(int ac, char **av)
{
	int	arg;
	int	err;

	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", STDERR_FILENO);
	if (ac == 1)
		exit(0);
	arg = ft_atol(av[1], &err);
	if (err)
	{
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(av[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		exit(arg);
	}
	if (ac > 2)
	{
		ft_putendl_fd("exit: too many arguments", STDERR_FILENO);
		return(1);
	}
	exit(arg % 256);
}
