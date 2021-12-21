/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 20:07:02 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/19 20:17:33 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*separate_pipe(t_cmd **ptr, t_cmd *cmd, char *line)
{
	t_cmd	*tmp;

	if (!ft_strncmp(cmd->token, "", 1))
	{
		cmd->token = append(cmd->token, '|');
		add_cmd(ptr, cmd);
	}
	else
	{
		add_cmd(ptr, cmd);
		tmp = init_cmd();
		tmp->token = append(tmp->token, '|');
		add_cmd(ptr, tmp);
	}
	return (line + 1);
}

static char	*separate_rdr_out(t_cmd **ptr, t_cmd *cmd, char *line)
{
	t_cmd	*tmp;

	if (!ft_strncmp(cmd->token, "", 1))
	{
		cmd->token = append(cmd->token, '>');
		if (!ft_strncmp(line, ">>", 2))
			cmd->token = append(cmd->token, '>');
		add_cmd(ptr, cmd);
	}
	else
	{
		add_cmd(ptr, cmd);
		tmp = init_cmd();
		tmp->token = append(tmp->token, '>');
		if (!ft_strncmp(line, ">>", 2))
			tmp->token = append(tmp->token, '>');
		add_cmd(ptr, tmp);
	}
	if (!ft_strncmp(line, ">>", 2))
		return (line + 2);
	return (line + 1);
}

static char	*separate_rdr_in(t_cmd **ptr, t_cmd *cmd, char *line)
{
	t_cmd	*tmp;

	if (!ft_strncmp(cmd->token, "", 1))
	{
		cmd->token = append(cmd->token, '<');
		if (!ft_strncmp(line, "<<", 2))
			cmd->token = append(cmd->token, '<');
		add_cmd(ptr, cmd);
	}
	else
	{
		add_cmd(ptr, cmd);
		tmp = init_cmd();
		tmp->token = append(tmp->token, '<');
		if (!ft_strncmp(line, "<<", 2))
			tmp->token = append(tmp->token, '<');
		add_cmd(ptr, tmp);
	}
	if (!ft_strncmp(line, "<<", 2))
		return (line + 2);
	return (line + 1);
}

char	*separate(t_cmd **ptr, t_cmd **cmd, char *line)
{
	if (is_space(*line))
		add_cmd(ptr, *cmd);
	else if (*line == '|')
		line = separate_pipe(ptr, *cmd, line);
	else if (*line == '>')
		line = separate_rdr_out(ptr, *cmd, line);
	else if (*line == '<')
		line = separate_rdr_in(ptr, *cmd, line);
	while (*line && is_space(*line))
		++line;
	if (*line)
		*cmd = init_cmd();
	else
		*cmd = NULL;
	return (line);
}
