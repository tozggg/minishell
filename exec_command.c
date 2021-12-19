/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:13:43 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/19 18:24:49 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "minishell.h"
#include "libft/libft.h"

void	child_process(char **av, t_rdinfo rd, t_pipeinfo pipeinfo, t_env **env)
{
#ifdef DEBUG
	printf("%s: read %d - write %d - tobefree %d\n", av[0],
		pipeinfo.read, pipeinfo.write, pipeinfo.unused);
#endif
	if (pipeinfo.write != STDOUT_FILENO)
		safe_close_readend(pipeinfo.unused);
	dup2(pipeinfo.read, STDIN_FILENO);
	if (pipeinfo.read != STDIN_FILENO)
		close(pipeinfo.read);
	dup2(pipeinfo.write, STDOUT_FILENO);
	if (pipeinfo.write != STDOUT_FILENO)
		close(pipeinfo.write);
	dup2(rd.write, STDOUT_FILENO);
	dup2(rd.read, STDIN_FILENO);
	if (ft_execvpe(av[0], av, env) != 0)
	{
		perror(av[0]);
		if (errno == ENOENT)
			exit(127);
		else
			exit(126);
	}
}

/* child process에서 cmd 실행
 * 리디렉션이 파이프보다 우선순위가 더 높으므로
 * 파이프를 먼저 적용 후 rdinfo가 지정한 대로 in/out을 overwrite
 * main process는 child가 종료될 때까지 대기
 * if cmd is builtin without pipe, run it in main process.
 * return exit_code * (-1) to distingish with other case.
 * if child process is created, return pid.
 * this pid's exit status will be real exit code of entire command.
*/
int	execute_command(t_cmd *node, t_rdinfo rd, t_pipeinfo pipeinfo, t_env **env)
{
	char	**av;
	pid_t	pid;

	av = listtostrarray(node);
#ifdef DEBUG
	int i = -1;
	while (av[++i] != 0)
		printf("arg[%d]:%s ", i, av[i]);
	printf("\n");
#endif
	if (av[0] == NULL)
	{
		free(av);
		return (0);
	}
	if (is_builtin(av[0]) && pipeinfo.read == 0 && pipeinfo.write == 1)
		return (exec_builtin_single(av, rd, env) * -1);
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		exit(1);
	}
	else if (pid == 0)
		child_process(av, rd, pipeinfo, env);
	// parent
	if (rd.write != STDOUT_FILENO)
		close(rd.write);
	if (rd.read != STDIN_FILENO)
		close(rd.read);
	free(av);
	return (pid);
}

/* 리디렉션 토큰이 존재한다면 rdinfo에 어디로 read,write할 것인지 저장 후
 * execute_command로 전달
 * 왼쪽부터 순차적으로 처리하되, file open 실패하면 중단
*/
int	command(t_cmd *node, t_pipeinfo pipeinfo, t_env **env)
{
	t_cmd		*head;
	t_rdinfo	rd;
	int			rdtype;

	rd = (t_rdinfo){STDIN_FILENO, STDOUT_FILENO};
	head = node;
	if (head->cmd_type == TYPE_INVALID)
		return (1 * -1);
	while (1)
	{
		rdtype = is_redirection_node(node);
		if (rdtype != NONE)
		{
			if (store_rdinfo(node, &rd, rdtype) < 0)
				return (1 * -1);
			node->cmd_type = TYPE_RDSIGN;
			node->next->cmd_type = TYPE_RDTARGET;
			node = node->next;
		}
		if (node->cmd_end)
			break ;
		node = node->next;
	}
	return (execute_command(head, rd, pipeinfo, env));
}
