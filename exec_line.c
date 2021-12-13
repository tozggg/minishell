/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:34:32 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/12 19:23:24 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"
#include "libft/libft.h"
#include "parse/tmp_listfunc.h"

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

// **envp 전달을 위해 arguments 압축
void exec_pipe(t_cmd *node, int read_fd, int write_fd)
{
	t_pipefd	pipefd;

	pipefd = (t_pipefd){read_fd, write_fd};
	command(node, pipefd);
}

// node부터 pipe_node까지를 한 단위로 끊어서 실행
// 첫 cmd의 입력은 STDIN, 마지막 cmd의 입력은 STDOUT
int	exec_line(t_cmd *node)
{
	t_cmd		*pipe_node;
	int			pfd[2];
	int			piperead;
	int			pipewrite;

	piperead= STDIN_FILENO;
	pipewrite = STDOUT_FILENO;
	while (1)
	{
		pipe_node = has_pipe(node);
		if (!pipe_node)
			break ;	//                pipewrite           piperead
		pipe(pfd);	// data written to pfd[1] is passed to pfd[0]
		pipewrite = pfd[1];	// when process1 writes data to pipewrite, then process2 will read it from piperead
		exec_pipe(node, piperead, pipewrite);
		if (piperead != STDIN_FILENO)
			close(piperead);
		close(pipewrite);
		piperead = pfd[0];
		node = pipe_node->next;
	}
	exec_pipe(node, piperead, STDOUT_FILENO);
	if (piperead != STDIN_FILENO)
		close(piperead);
	return (0);
}
