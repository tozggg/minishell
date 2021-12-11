/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:34:32 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/11 18:44:55 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"
#include "libft/libft.h"

t_list	*has_pipe(t_list *node)
{
	while (node != NULL)
	{
		if (((char *)node->content)[0] == '|')
			return (node);
		node = node->next;
	}
	return (0);
}

int	exec_line(t_list *node)
{
	t_list		*pipe_node;
	t_list		*remainder;
	int			pfd[2];
	t_pipefd	pipefd;

	pipefd = (t_pipefd){STDIN_FILENO, STDOUT_FILENO};
	remainder = node;
	while (1)
	{
		pipe_node = has_pipe(node);
		if (!pipe_node)
			break ;
		remainder = pipe_node->next;
		ft_lstcut(node, pipe_node);
		pipe(pfd);
		pipefd.write_fd = pfd[1];
		command(node, pipefd.read_fd, pipefd.write_fd);
		if (pipefd.read_fd != STDIN_FILENO)
			close(pipefd.read_fd);
		close(pipefd.write_fd);
		pipefd.read_fd = pfd[0];
		node = remainder;
	}
	command(remainder, pipefd.read_fd, STDOUT_FILENO);
	if (pipefd.read_fd != STDIN_FILENO)
		close(pipefd.read_fd);
	return (0);
}
