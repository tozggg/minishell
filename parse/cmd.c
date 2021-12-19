/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 19:59:29 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/19 16:22:02 by kanlee           ###   ########.fr       */
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

int	check_cmd(t_cmd *cmd)
{
	int	prev_is_pipe;
	int	prev_is_rdr;

	if (ft_strequ(cmd->token, "|"))
		return (SYNTAX_ERR);
	prev_is_pipe = 0;
	prev_is_rdr = 0;
	while (cmd)
	{
		if ((prev_is_pipe && ft_strequ(cmd->token, "|")) || \
	(prev_is_rdr && (ft_strequ(cmd->token, "|") || is_redirection_node(cmd))))
			return (SYNTAX_ERR);
		if (ft_strequ(cmd->token, "|"))
			prev_is_pipe = 1;
		else
			prev_is_pipe = 0;
		if (is_redirection_node(cmd))
			prev_is_rdr = 1;
		else
			prev_is_rdr = 0;
		if (!(cmd->next) && ft_strequ(cmd->token, "|"))
			return (SYNTAX_ERR);
		cmd = cmd->next;
	}
	return (0);
}
