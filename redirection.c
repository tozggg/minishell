/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 20:53:33 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/17 11:54:45 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <unistd.h>
#include <readline/readline.h>
#include "minishell.h"
#include "libft/libft.h"
#include "parse/tmp_listfunc.h"

int	is_redirection_node(t_cmd *node)
{
	if (ft_strequ(node->token, ">"))
		return (RD_WRITE);
	if (ft_strequ(node->token, ">>"))
		return (RD_APPEND);
	if (ft_strequ(node->token, "<"))
		return (RD_READ);
	if (ft_strequ(node->token, "<<"))
		return (RD_HEREDOC);
	return (NONE);
}

int	is_target_valid(t_cmd *node)
{
	int		fd;
	char	*rdtarget;

	rdtarget = node->next->token;
	if (is_redirection_node(node) == RD_READ)
	{
		fd = open(rdtarget, O_RDONLY);
		if (fd < 0)
		{
			perror(rdtarget);
			return (0);
		}
	}
	if (is_redirection_node(node) == RD_WRITE
		|| is_redirection_node(node) == RD_APPEND)
	{
		fd = open(rdtarget, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror(rdtarget);
			return (0);
		}
	}
	close(fd);
	return (1);
}

void	chk_rdtarget(t_cmd *node)
{
	t_cmd	*head;
	int		rdtype;

	while (node != NULL)
	{
		head = node;
		while (node != NULL && !ft_strequ(node->token, "|"))
		{
			rdtype = is_redirection_node(node);
			if (rdtype == RD_READ || rdtype == RD_WRITE || rdtype == RD_APPEND)
			{
				if (!is_target_valid(node))
				{
					head->cmd_type = TYPE_INVALID;
					while (node != NULL && !ft_strequ(node->token, "|"))
						node = node->next;
				}
			}
			if (node != NULL)
				node = node->next;
		}
		if (node != NULL)
			node = node->next;
	}
}

int	open_target(int rdtype, char *rdtarget)
{
	int	fd;

	if (rdtype == RD_WRITE)
		fd = open(rdtarget, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (rdtype == RD_APPEND)
		fd = open(rdtarget, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (rdtype == RD_READ)
		fd = open(rdtarget, O_RDONLY);
	if (rdtype == RD_HEREDOC)
		fd = ft_atoi(rdtarget);
	return (fd);
}

int	store_rdinfo(t_cmd *node, t_rdinfo *rd, int rdtype)
{
	char	*rdtarget;
	int		fd;

	rdtarget = node->next->token;
	fd = open_target(rdtype, rdtarget);
	if (fd < 0)
	{
		perror(rdtarget);
		return (-1);
	}
	if (rdtype == RD_WRITE || rdtype == RD_APPEND)
	{
		// if write redirection is already set, ignore former
		if (rd->write != STDOUT_FILENO)
			close(rd->write);
		rd->write = fd;
	}
	if (rdtype == RD_READ || rdtype == RD_HEREDOC)
	{
		// if read redirection is already set, ignore former
		if (rd->read != STDIN_FILENO)
			close(rd->read);
		rd->read = fd;
	}
	return (0);
}
