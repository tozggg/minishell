/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 03:40:42 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/19 17:14:50 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>
#include <string.h>

void	error_out(char *str)
{
	ft_putendl_fd(str, STDERR_FILENO);
	exit(1);
}

void	err_print(int err_flag)
{
	if (err_flag == QUOTE_ERR)
		ft_putendl_fd("minishell: the number of quote is odd", STDERR_FILENO);
	if (err_flag == SYNTAX_ERR)
		ft_putendl_fd("minishell: invalid syntax", STDERR_FILENO);
}

void	errno_print(int errnum)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putendl_fd(strerror(errnum), STDERR_FILENO);
}

void	identifier_err_print(void)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putendl_fd("not a valid identifier", STDERR_FILENO);
}
