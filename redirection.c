/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 20:53:33 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/13 21:48:01 by kanlee           ###   ########.fr       */
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

int	write_to_tmpfile(int fd, char *limit)
{
	char	*input;

	while (1)
	{
		input = readline(">");
		if (input == NULL || ft_strequ(input, limit))
		{
			if (!input)
			{
				printf("warning: heredoc delimited by end-of-file");
				printf(" (wanted '%s')\n", limit);
			}
			else
				free(input);
			break ;
		ft_putendl_fd(input, fd);
		free(input);
	}
	return (0);
}

// open tmp file for write
//   print heredoc prompt
//   read input from stdin
// while input==limitstr
// close tmpfile
// open it again read-only
// unlink() --- tmpfile will be deleted after it's closed
// read input from tmpfile
// print warning when reached EOF before Limitstr found.
int	read_heredoc(t_cmd *node)
{
	int		fd;
	char	*limit;

	limit = node->next->token;
	fd = open("tmp_for_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	write_to_tmpfile(fd, limit);
	close(fd);
	fd = open("tmp_for_heredoc", O_RDONLY);
	unlink("tmp_for_heredoc");
	free(node->next->token);
	node->next->token = ft_strdup(ft_itoa(fd));
	return (0);
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
	if (rdtype == RD_READ || rdtype == RD_HEREDOC)
	{
		// if read redirection is already set, ignore former
		if (rd->read_fd != STDIN_FILENO)
			close(rd->read_fd);
		rd->read_fd = fd;
	}
	return (0);
}
