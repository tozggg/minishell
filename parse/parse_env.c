/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 20:08:55 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/20 01:32:27 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*sandwich(char *token, char *env_value, int env_len, int flag)
{
	char	*res;
	int		len;
	int		i;

	len = ft_strlen(token) + env_len - 1;
	res = (char *)malloc(sizeof(char) * (len + 1));
	i = -1;
	while (++i < len)
	{
		if (flag == 1)
			res[i] = token[i - env_len + 1];
		else if (token[i] == '$' && flag == 0)
		{
			while (*env_value)
				res[i++] = *(env_value++);
			flag = 1;
			--i;
		}
		else
			res[i] = token[i];
	}
	res[i] = 0;
	free(token);
	return (res);
}

static char	*parse_env_not_key(char *token, char *key)
{
	char	*res;
	char	*tmp;

	if (ft_strequ(key, "?"))
	{
		tmp = ft_itoa(g_exit_status);
		res = sandwich(token, tmp, ft_strlen(tmp), 0);
		free(tmp);
	}
	else
		res = sandwich(token, key, ft_strlen(key), 0);
	return (res);
}

void	parse_env(t_cmd *cmd, t_env *env)
{
	t_env_key	*tmp;
	char		*value;

	while (cmd)
	{
		tmp = cmd->env_key;
		while (tmp)
		{
			if (tmp->is_key == 1)
			{
				value = get_value(env, tmp->key);
				if (value)
					cmd->token = sandwich(cmd->token, \
									value, ft_strlen(value), 0);
				else
					cmd->token = sandwich(cmd->token, "", 0, 0);
			}
			else if (tmp->is_key == 0)
				cmd->token = parse_env_not_key(cmd->token, tmp->key);
			tmp = tmp->next;
		}
		cmd = cmd->next;
	}
}
