/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:13:43 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/12 19:27:11 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft/libft.h"
#include "parse/tmp_listfunc.h"

int	execute_command(t_cmd *node, int piperead, int pipewrite)
{
	char	*cmd;
	char	**av;
	pid_t	pid;
	int		stdout_bak;

	av = listtostrarray(node);
	cmd = av[0];
#ifdef DEBUG
	printf("cmd: %s\n", cmd);
	int i = 0;
	while (av[i] != 0)
		printf("arg[%d]: %s\n", i, av[i++]);
#endif
	//TODO: builtin commands without pipes are executed inside main process
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		return (-1);
	}
	else if (pid == 0)
	{
		//TODO: implement redirection
		if (cmd != NULL)
		{
			stdout_bak = dup(STDOUT_FILENO);
			if (piperead != STDIN_FILENO)
				dup2(piperead, STDIN_FILENO);
			if (pipewrite != STDOUT_FILENO)
				dup2(pipewrite, STDOUT_FILENO);
			if (execvp(cmd, av) == -1) //TODO: make ft_execvpe() using execve  
			{
				dup2(stdout_bak, STDOUT_FILENO);
				printf("%s: command not found\n", cmd);
				exit(1);
			}
		}
	}
	waitpid(pid, NULL, 0);
	free(av);
	return (0);
}

int	is_redirection_node(t_cmd *node)
{
	if (ft_strequ(node->token, ">"))
		return (1);
	if (ft_strequ(node->token, ">>"))
		return (1);
	if (ft_strequ(node->token, "<"))
		return (1);
	if (ft_strequ(node->token, "<<"))
		return (1);
	return (0);
}

int	command(t_cmd *node, int piperead, int pipewrite)
{
	t_cmd	*head;

	head = node;
	while (1)
	{
		if (is_redirection_node(node))
		// TODO: save redirection info and pass it to execute_cmd()
		{
			node->cmd_type = TYPE_RDSIGN;
			node->next->cmd_type = TYPE_RDTARGET;
			node = node->next;
		}
		if (node->cmd_end)
			break ;
		node = node->next;
	}
#ifdef DEBUG
	printf("read from %d - write to %d\n", piperead, pipewrite);
#endif
	execute_command(head, piperead, pipewrite);
	return (0);
}
