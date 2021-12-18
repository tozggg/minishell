/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_listfunc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:59:27 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/18 15:53:55 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <termios.h>
#include <term.h>
#include "tmp_listfunc.h"
#include "../libft/libft.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
	{
		perror("malloc");
		exit(1);
	}
	cmd->token = ft_strdup("");
	cmd->cmd_type = 0;
	cmd->cmd_end = 0;
	cmd->env_key = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	destroy_cmd(t_cmd **ptr)
{
	t_cmd		*cmd;
	t_cmd		*tmp_c;
	t_env_key	*tmp_e;

	cmd = *ptr;
	while (cmd)
	{
		tmp_c = cmd->next;
		while (cmd->env_key)
		{
			tmp_e = cmd->env_key->next;
			free(cmd->env_key->key);
			free(cmd->env_key);
			cmd->env_key = tmp_e;
		}
		free(cmd->token);
		free(cmd);
		cmd = tmp_c;
	}
	*ptr = NULL;
}

void	add_cmd(t_cmd **ptr, t_cmd *cmd)
{
	t_cmd	*tmp;

	if (*ptr == NULL)
		*ptr = cmd;
	else
	{
		tmp = *ptr;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = cmd;
	}
}

void	add_env_key(t_cmd *cmd, t_env_key *env_key)
{
	t_env_key	*tmp;

	if (cmd->env_key == NULL)
		cmd->env_key = env_key;
	else
	{
		tmp = cmd->env_key;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = env_key;
	}
}

t_env_key	*init_env_key(void)
{
	t_env_key	*env_key;

	env_key = (t_env_key *)malloc(sizeof(t_env_key));
	if (!env_key)
	{
		perror("malloc");
		exit(1);
	}
	env_key->is_key = 0;
	env_key->key = ft_strdup("");
	env_key->next = NULL;
	return (env_key);
}

