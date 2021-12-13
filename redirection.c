/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 20:53:33 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/12 19:36:12 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include "minishell.h"
#include "libft/libft.h"
#include "parse/tmp_listfunc.h"

int	open_target(int rdtype, char *rdtarget)
{
	int	fd;

	if (rdtype == RD_WRITE)
		fd = open(rdtarget, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (rdtype == RD_APPEND)
		fd = open(rdtarget, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (rdtype == RD_READ)
		fd = open(rdtarget, O_RDONLY);
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
		if (errno == EISDIR)
			perror("ITS DIRECTORY");
		return (-1);
	}
	if (rdtype == RD_WRITE || rdtype == RD_APPEND)
	{
		// if write redirection is already set, ignore former
		if (rd->write_fd != STDOUT_FILENO)
			close(rd->write_fd);
		rd->write_fd = fd;
	}
	if (rdtype == RD_READ)
	{
		// if read redirection is already set, ignore former
		if (rd->read_fd != STDIN_FILENO)
			close(rd->read_fd);
		rd->read_fd = fd;
	}
	return (0);
}
