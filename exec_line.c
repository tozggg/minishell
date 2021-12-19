/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:34:32 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/19 19:48:02 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft/libft.h"

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

/* pfd[0] should be closed in child process
*/
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

int	refine(int exit_code)
{
	if (exit_code > 0)
		exit_code = monitor_child(exit_code);
	else
		exit_code *= -1;
#ifdef DEBUG
	printf("exit code:[%d]\n", exit_code);
#endif
	return (exit_code);
}

/* node부터 pipe_node까지를 한 단위로 끊어서 실행
 * 첫 cmd의 입력은 STDIN, 마지막 cmd의 입력은 STDOUT
 * exec_command will return nonzero if child process is created.
 * if not, return value is -1 * real exit_code.
*/
int	exec_line(t_cmd *node, t_env **env, int exit_code)
{
	t_cmd		*pipe_node;
	int			pfd[2];
	int			read_prev;

	// before executing, read HEREDOC first as bash does it.
	// echo asdf > outfile | cat << HERE
	// will not run echo or create outfile until heredoc input is completed.
	exit_code = chk_heredoc(node);
	if (exit_code != 0)
		return (exit_code);
	// this mimics bash 4.4.20 behavior. Needs more tests on 3.2.57 of Cluster Mac.
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
	return (refine(exit_code));
}
