/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 19:59:29 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/27 18:05:53 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../minishell.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		error_out("malloc error");
	cmd->token = ft_strdup("");
	cmd->has_quote = 0;
	cmd->cmd_type = 0;
	cmd->cmd_end = 0;
	cmd->env_key = NULL;
	cmd->next = NULL;
	return (cmd);
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
