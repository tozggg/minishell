/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 11:51:31 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/17 11:53:46 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
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
		}
		ft_putendl_fd(input, fd);
		free(input);
	}
	return (0);
}

int	open_available(char **tmpfilename)
{
	int		fd;
	int		i;
	char	*idxstr;

	i = 0;
	while (i < 65535)
	{
		idxstr = ft_itoa(i);
		*tmpfilename = ft_strjoin("/tmp/tmp_for_heredoc_", idxstr);
		free(idxstr);
		fd = open(*tmpfilename, O_WRONLY | O_CREAT | O_EXCL, 0644);
		if (fd > 0)
			return (fd);
		free(*tmpfilename);
		i++;
	}
	return (-1);
}

// open tmp file for write
//       return -1 if open failed.
//   print heredoc prompt
//   read input from stdin, write to tmpfile
// while input==limitstr
//   print warning when reached EOF before Limitstr found.
// close tmpfile
// open it again read-only
// unlink() --- tmpfile will be deleted after it's closed
// change RD_TARGET content to readable fd.
int	read_heredoc(t_cmd *node)
{
	int		fd;
	char	*limit;
	char	*tmpfilename;

	limit = node->next->token;
	fd = open_available(&tmpfilename);
	if (fd < 0)
	{
		printf("Fatal: can't create tmpfile for heredoc\n");
		return (-1);
	}
	write_to_tmpfile(fd, limit);
	close(fd);
	fd = open(tmpfilename, O_RDONLY);
	unlink(tmpfilename);
	free(node->next->token);
	node->next->token = ft_strdup(ft_itoa(fd));
	return (0);
}
