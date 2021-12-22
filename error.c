/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 03:40:42 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/22 19:41:24 by taejkim          ###   ########.fr       */
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
	if (!err_flag || err_flag == EMPTY_LINE)
		return ;
	else if (err_flag == UNSPEC_CHAR_ERR)
		ft_putendl_fd("syntax error: unspecified special characters", \
			STDERR_FILENO);
	else if (err_flag == QUOTE_ERR)
		ft_putendl_fd("syntax error: unclosed quotes", STDERR_FILENO);
	else if (err_flag == NEAR_PIPE_ERR)
		ft_putendl_fd("syntax error: near unexpected token `|'", STDERR_FILENO);
	else if (err_flag == NEAR_WRITE_ERR)
		ft_putendl_fd("syntax error: near unexpected token `>'", STDERR_FILENO);
	else if (err_flag == NEAR_READ_ERR)
		ft_putendl_fd("syntax error: near unexpected token `<'", STDERR_FILENO);
	else if (err_flag == NEAR_APPEND_ERR)
		ft_putendl_fd("syntax error: near unexpected token `>>'", \
			STDERR_FILENO);
	else if (err_flag == NEAR_HEREDOC_ERR)
		ft_putendl_fd("syntax error: near unexpected token `<<'", \
			STDERR_FILENO);
	else if (err_flag == NEAR_NEWRINE_ERR)
		ft_putendl_fd("syntax error: near unexpected token `newline'", \
			STDERR_FILENO);
	g_exit_status = 1;
}

void	errno_print(int errnum, char *place)
{
	ft_putstr_fd(place, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errnum), STDERR_FILENO);
}

void	identifier_err_print(char *place)
{
	ft_putstr_fd("`", STDERR_FILENO);
	ft_putstr_fd(place, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

int	home_error_print(void)
{
	int	exit_status;

	ft_putendl_fd("minishell: HOME not set", STDERR_FILENO);
	exit_status = 1;
	return (exit_status);
}
