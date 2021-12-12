/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:34:32 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/12 19:30:07 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"
#include "libft/libft.h"
#include "parse/tmp_listfunc.h"

t_cmd	*has_pipe(t_cmd *node)
{
	while (node != NULL)
	{
		if (ft_strequ(node->token, "|"))
			return (node);
		node = node->next;
	}
	return (0);
}

int	exec_line(t_cmd *node)
{
	t_cmd		*pipe_node;
	int			pfd[2];
	t_pipefd	pipefd;

	pipefd = (t_pipefd){STDIN_FILENO, STDOUT_FILENO};
	while (1)
	{
		pipe_node = has_pipe(node);
		if (!pipe_node)
			break ;
		pipe(pfd);
		pipefd.write_fd = pfd[1];
		command(node, pipefd.read_fd, pipefd.write_fd);
		if (pipefd.read_fd != STDIN_FILENO)
			close(pipefd.read_fd);
		close(pipefd.write_fd);
		pipefd.read_fd = pfd[0];
		node = pipe_node->next;
	}
	command(node, pipefd.read_fd, STDOUT_FILENO);
	if (pipefd.read_fd != STDIN_FILENO)
		close(pipefd.read_fd);
	return (0);
}
