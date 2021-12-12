/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:13:43 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/12 21:50:13 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft/libft.h"
#include "parse/tmp_listfunc.h"

int	execute_command(t_cmd *node, t_rdinfo rd, int piperead, int pipewrite)
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
////////// redirection /////////
			if (rd.write_fd != STDOUT_FILENO)
				dup2(rd.write_fd, STDOUT_FILENO);
////////////////////////////////

			if (execvp(cmd, av) == -1) //TODO: make ft_execvpe() using execve  
			{
				dup2(stdout_bak, STDOUT_FILENO);
				printf("%s: command not found\n", cmd);
				exit(1);
			}
		}
	}
	waitpid(pid, NULL, 0);
	if (rd.write_fd != STDOUT_FILENO)
		close(rd.write_fd);
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

int	command(t_cmd *node, int piperead, int pipewrite)
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
		// TODO: save redirection info and pass it to execute_cmd()
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
	printf("read from %d - write to %d\n", piperead, pipewrite);
#endif
	execute_command(head, rd, piperead, pipewrite);
	return (0);
}
