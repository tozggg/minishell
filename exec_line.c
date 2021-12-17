/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:34:32 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/17 08:22:57 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft/libft.h"
#include "parse/tmp_listfunc.h"
#include <stdio.h>

// node 이후에 pipe가 존재하는지 확인
// pipe 바로 전 또는 list_end까지가 하나의 커맨드
t_cmd	*has_pipe(t_cmd *node)
{
	while (node != NULL)
	{
		if (node->next == NULL || ft_strequ(node->next->token, "|"))
		{
			node->cmd_end = 1;
			return (node->next);
		}
		node = node->next;
	}
	return (NULL);
}

t_cmd	*has_heredoc(t_cmd *node)
{
	while (node != NULL)
	{
		if (node->next != NULL && ft_strequ(node->next->token, "<<"))
			return (node->next);
		node = node->next;
	}
	return (NULL);
}

int	chk_heredoc(t_cmd *node)
{
	t_cmd	*heredoc_node;

	while (1)
	{
		heredoc_node = has_heredoc(node);
		if (!heredoc_node)
			break ;
		if (read_heredoc(heredoc_node) < 0)
			return (-1);
		node = heredoc_node->next;
	}
	return (0);
}

// pfd[0] should be closed in child process
void	exec_pipe(t_cmd *node, int read_fd, int *pfd)
{
	t_pipeinfo	pipeinfo;

	pipeinfo.read = read_fd;
	pipeinfo.write = pfd[1];
	pipeinfo.unused = pfd[0];
	if (command(node, pipeinfo) < 0)
		printf("FIXME: returned nonzero but not stored\n"); // FIXME
}

// node부터 pipe_node까지를 한 단위로 끊어서 실행
// 첫 cmd의 입력은 STDIN, 마지막 cmd의 입력은 STDOUT
int	exec_line(t_cmd *node)
{
	t_cmd		*pipe_node;
	int			pfd[2];
	int			read_prev;
	int			wstatus;

	// before executing, read HEREDOC first as bash does it.
	// echo asdf > outfile | cat << HERE
	// will not run echo or create outfile until heredoc input is completed.
	if (chk_heredoc(node) < 0)
		return (-1);
	chk_rdtarget(node);
	read_prev = STDIN_FILENO;
	while (1)
	{
		pipe_node = has_pipe(node);
		if (!pipe_node)
			break ;
		// found pipe. this cmd will write to new pipe's write-end, and read from prev pipe's read-end.
		pipe(pfd);
		exec_pipe(node, read_prev, pfd); // new pipe's read-end is not needed in this cmd, so it should be closed in child process
		safe_close_readend(read_prev);
		close(pfd[1]);
		read_prev = pfd[0];
		node = pipe_node->next;
	}
	pfd[1] = STDOUT_FILENO;
	exec_pipe(node, read_prev, pfd);
	safe_close_readend(read_prev);
	while (waitpid(-1, &wstatus, 0) > 0)
		printf("exit status=%d\n", WEXITSTATUS(wstatus));
	return (0);
}
