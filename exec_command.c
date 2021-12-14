/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:13:43 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/14 17:49:44 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"
#include "libft/libft.h"
#include "parse/tmp_listfunc.h"

// child process에서 cmd 실행
// 리디렉션이 파이프보다 우선순위가 더 높으므로
// 파이프를 먼저 적용 후 rdinfo가 지정한 대로 in/out을 overwrite
// main process는 child가 종료될 때까지 대기
int	execute_command(t_cmd *node, t_rdinfo rd, t_pipefd pipefd)
{
	char	*cmd;
	char	**av;
	pid_t	pid;
	int		stdout_bak;

	av = listtostrarray(node);
	cmd = av[0];			// FIXME: cmd == NULL
#ifdef DEBUG
	printf("cmd: %s\n", cmd);
	int i = 0;
	while (av[i] != 0)
		printf("arg[%d]: %s\n", i, av[i++]);
#endif
	//TODO: builtin commands without pipes are executed inside main process
	// if (is_builtin(cmd) && pipefd.read_fd == STDIN_FILENO && pipefd.write_fd == STDOUT_FILENO)
	//		return do_sth();
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		exit(-1);
	}
	else if (pid == 0)
	{
		if (cmd != NULL)
		{
			stdout_bak = dup(STDOUT_FILENO);
			if (pipefd.read_fd != STDIN_FILENO)
				dup2(pipefd.read_fd, STDIN_FILENO);
			if (pipefd.write_fd != STDOUT_FILENO)
				dup2(pipefd.write_fd, STDOUT_FILENO);
////////// redirection /////////
			if (rd.write_fd != STDOUT_FILENO)
				dup2(rd.write_fd, STDOUT_FILENO);
			if (rd.read_fd != STDIN_FILENO)
				dup2(rd.read_fd, STDIN_FILENO);
////////////////////////////////

			if (execvp(cmd, av) == -1) //TODO: make ft_execvpe() using execve  
			{
				dup2(stdout_bak, STDOUT_FILENO);
				printf("%s: command not found\n", cmd);
				exit(1);
			}
		}
	}
	if (rd.write_fd != STDOUT_FILENO)
		close(rd.write_fd);
	if (rd.read_fd != STDIN_FILENO)
		close(rd.read_fd);
	free(av);
	return (0);
}

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

// 리디렉션 토큰이 존재한다면 rdinfo에 어디로 read,write할 것인지 저장 후 execute_command로 전달
// 왼쪽부터 순차적으로 처리하되, file open 실패하면 중단
int	command(t_cmd *node, t_pipefd pipefd)
{
	t_cmd		*head;
	t_rdinfo	rd;
	int			rdtype;

	rd = (t_rdinfo){STDIN_FILENO, STDOUT_FILENO};
	head = node;
	while (1)
	{
		rdtype = is_redirection_node(node);
		if (rdtype != NONE)
		{
			if (store_rdinfo(node, &rd, rdtype) < 0)
				return (-1);
			node->cmd_type = TYPE_RDSIGN;
			node->next->cmd_type = TYPE_RDTARGET;
			node = node->next;
		}
		if (node->cmd_end)
			break ;
		node = node->next;
	}
#ifdef DEBUG
	printf("read from %d - write to %d\n", pipefd.read_fd, pipefd.write_fd);
#endif
	execute_command(head, rd, pipefd);
	return (0);
}
