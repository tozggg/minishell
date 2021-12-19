/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 11:51:31 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/19 20:40:11 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_in_heredoc_handler(int sig)
{
	if (sig == SIGINT)
		close(STDIN_FILENO);
}

int	cleanup(int stdin_bak)
{
	signal(SIGINT, sig_handler);
	if (isatty(STDIN_FILENO))
	{
		printf("heredoc completed\n");
		return (0);
	}
	else
	{
		printf("heredoc interrupted\n");
		dup2(stdin_bak, STDIN_FILENO);
		return (130);
	}
}

/* We're in second readline.
 * When user press ctrl-c, we should stop readline and return to prompt.
 * To do this, replace signal handler temporarily and restore it when
 *   heredoc input is completed.
 * New sighandler will close STDIN to break current readline.
 * if input == NULL && STDIN is not terminal, it means we met SIGINT.
 * in this case, restore STDIN and return 130.
 * In general situation, write input to tmpfile and return 0.
*/
int	write_to_tmpfile(int fd, char *limit)
{
	char	*input;
	int		stdin_bak;

	stdin_bak = dup(STDIN_FILENO);
	signal(SIGINT, sigint_in_heredoc_handler);
	while (1)
	{
		input = readline(">");
		if (input == NULL || ft_strequ(input, limit))
		{
			if (!input && isatty(STDIN_FILENO))
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
	return (cleanup(stdin_bak));
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

/* open tmp file for write
 *   print heredoc prompt
 *   read input from stdin, write to tmpfile
 * while input==limitstr
 * print warning when reached EOF before Limitstr found.
 * close tmpfile
 * open it again read-only
 * unlink() --- tmpfile will be deleted after it's closed
 * change RD_TARGET content to readable fd.
 * return 1 if open failed or 130 if interrupted.
*/
int	read_heredoc(t_cmd *node, char *limit)
{
	int		fd;
	char	*tmpfilename;
	int		ret;

	fd = open_available(&tmpfilename);
	if (fd < 0)
	{
		printf("Fatal: can't create tmpfile for heredoc\n");
		return (1);
	}
	ret = write_to_tmpfile(fd, limit);
	close(fd);
	if (ret != 0)
	{
		unlink(tmpfilename);
		free(tmpfilename);
		return (ret);
	}
	fd = open(tmpfilename, O_RDONLY);
	unlink(tmpfilename);
	free(tmpfilename);
	free(node->next->token);
	node->next->token = ft_itoa(fd);
	return (0);
}
