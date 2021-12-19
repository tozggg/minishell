/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:34:32 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/19 16:40:34 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft/libft.h"
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

/* read_heredoc returns 0 if success, 1 if tmpfile creation failed,
 * 130 if interrupted by SIGINT
*/
int	chk_heredoc(t_cmd *node)
{
	t_cmd	*heredoc_node;
	int		ret;

	while (1)
	{
		heredoc_node = has_heredoc(node);
		if (!heredoc_node)
			break ;
		ret = read_heredoc(heredoc_node, heredoc_node->next->token);
		if (ret != 0)
			return (ret);
		node = heredoc_node->next;
	}
	return (0);
}

// pfd[0] should be closed in child process
int	exec_pipe(t_cmd *node, int read_fd, int *pfd, t_env **env)
{
	t_pipeinfo	pipeinfo;

	pipeinfo.read = read_fd;
	pipeinfo.write = pfd[1];
	pipeinfo.unused = pfd[0];
	return (command(node, pipeinfo, env));
}

int	monitor_child(pid_t lastpid)
{
	int	exit_code;
	int	wstatus;
	int	finished_pid;

	while (1)
	{
		finished_pid = waitpid(-1, &wstatus, 0);
		if (finished_pid == lastpid)
		{
			exit_code = WEXITSTATUS(wstatus);
			if (WIFSIGNALED(wstatus))
			{
				exit_code = WTERMSIG(wstatus) + 128;
				if (exit_code == 131)
					printf("Quit\n");
				else if (exit_code != 130)
					printf("Terminated by signal %d\n", exit_code - 128);
			}
		}
		if (finished_pid < 0)
			break ;
	}
	return (exit_code);
}

// node부터 pipe_node까지를 한 단위로 끊어서 실행
// 첫 cmd의 입력은 STDIN, 마지막 cmd의 입력은 STDOUT
// exec_command will return nonzero if child process is created.
// if not, return value is -1 * real exit_code.
int	exec_line(t_cmd *node, t_env **env)
{
	t_cmd		*pipe_node;
	int			pfd[2];
	int			read_prev;
	int			exit_code;

	// before executing, read HEREDOC first as bash does it.
	// echo asdf > outfile | cat << HERE
	// will not run echo or create outfile until heredoc input is completed.
	exit_code = chk_heredoc(node);
	if (exit_code != 0)
		return (exit_code);
	chk_rdtarget(node);
	read_prev = STDIN_FILENO;
	while (1)
	{
		pipe_node = has_pipe(node);
		if (!pipe_node)
			break ;
		// found pipe. this cmd will write to new pipe's write-end, and read from prev pipe's read-end.
		pipe(pfd);
		exec_pipe(node, read_prev, pfd, env); // new pipe's read-end is not needed in this cmd, so it should be closed in child process
		safe_close_readend(read_prev);
		close(pfd[1]);
		read_prev = pfd[0];
		node = pipe_node->next;
	}
	pfd[1] = STDOUT_FILENO;
	exit_code = exec_pipe(node, read_prev, pfd, env);
	safe_close_readend(read_prev);
	if (exit_code > 0)
		exit_code = monitor_child(exit_code);
	else
		exit_code *= -1;
	printf("exit code = %d\n", exit_code);
	return (exit_code);
}
