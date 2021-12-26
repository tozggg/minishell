/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 20:07:32 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/24 04:09:46 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*envp_decision(t_cmd *cmd, char *line)
{
	t_env_key	*env_key;

	env_key = init_env_key();
	++line;
	if (!is_allow_envpname(*line))
	{
		if (*line == '?')
		{
			env_key->key = append(env_key->key, '?');
			++line;
		}
		else if (*line != '\'' && *line != '\"')
			env_key->key = append(env_key->key, '$');
		add_env_key(cmd, env_key);
	}
	else
	{
		env_key->is_key = 1;
		while (*line && is_allow_envpname(*line))
			env_key->key = append(env_key->key, *(line++));
		add_env_key(cmd, env_key);
	}
	cmd->token = append(cmd->token, '$');
	return (line);
}

static char	*small_quote_envp_decision(t_cmd *cmd, char *line)
{
	t_env_key	*env_key;

	env_key = init_env_key();
	env_key->key = append(env_key->key, '$');
	add_env_key(cmd, env_key);
	cmd->token = append(cmd->token, '$');
	return (line + 1);
}

static char	*big_quote_envp_decision(t_cmd *cmd, char *line)
{
	t_env_key	*env_key;

	env_key = init_env_key();
	++line;
	if (!is_allow_envpname(*line))
	{
		if (*line == '?')
		{
			env_key->key = append(env_key->key, '?');
			++line;
		}	
		else
			env_key->key = append(env_key->key, '$');
		add_env_key(cmd, env_key);
	}
	else
	{
		env_key->is_key = 1;
		while (*line && is_allow_envpname(*line))
			env_key->key = append(env_key->key, *(line++));
		add_env_key(cmd, env_key);
	}
	cmd->token = append(cmd->token, '$');
	return (line);
}

static char	*start_quote(t_cmd *cmd, char *line, int *err_flag)
{
	char	quote;

	quote = *line;
	++line;
	while (*line)
	{
		if (*line == quote)
			return (line + 1);
		if (*line == '$')
		{
			if (quote == '\"')
				line = big_quote_envp_decision(cmd, line);
			else
				line = small_quote_envp_decision(cmd, line);
		}
		else
			cmd->token = append(cmd->token, *(line++));
	}
	*err_flag = QUOTE_ERR;
	return (line);
}

void	parse(t_cmd **ptr, char *line, int *err_flag)
{
	t_cmd	*cmd;

	if (*ptr)
		destroy_cmd(ptr);
	if (!(*line))
		*err_flag = EMPTY_LINE;
	cmd = init_cmd();
	while (*line && is_space(*line))
		++line;
	while (*line)
	{
		if (is_unspecified_char(*line))
			*err_flag = UNSPEC_CHAR_ERR;
		if (is_space(*line) || is_separator(*line))
			line = separate(ptr, &cmd, line);
		else if (*line == '\"' || *line == '\'')
			line = start_quote(cmd, line, err_flag);
		else if (*line == '$')
			line = envp_decision(cmd, line);
		else
			cmd->token = append(cmd->token, *(line++));
	}
	add_cmd(ptr, cmd);
}
