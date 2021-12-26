/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 20:08:55 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/26 23:19:34 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../minishell.h"

static int	splicing(char *res, char *env_value, int *pass, int *flag)
{
	int	i;

	i = 0;
	if (*pass == 0)
	{
		while (*env_value)
			res[i++] = *(env_value++);
		*flag = 1;
		--i;
	}
	else
	{
		res[i] = '$';
		--(*pass);
	}
	return (i);
}

static char	*sandwich(char *token, char *env_value, int pass, int flag)
{
	char	*res;
	int		env_len;
	int		len;
	int		i;

	env_len = ft_strlen(env_value);
	len = ft_strlen(token) + env_len - 1;
	res = (char *)malloc(sizeof(char) * (len + 1));
	i = -1;
	while (++i < len)
	{
		if (flag == 1)
			res[i] = token[i - env_len + 1];
		else if (token[i] == '$' && flag == 0)
			i += splicing(&res[i], env_value, &pass, &flag);
		else
			res[i] = token[i];
	}
	res[i] = 0;
	free(token);
	return (res);
}

static char	*parse_env_not_key(char *token, char *key, int *pass)
{
	char	*res;
	char	*tmp;

	if (ft_strequ(key, "?"))
	{
		tmp = ft_itoa(g_exit_status);
		res = sandwich(token, tmp, *pass, 0);
		free(tmp);
	}
	else
		res = sandwich(token, key, *pass, 0);
	if (ft_strequ(key, "$"))
		++(*pass);
	return (res);
}

void	parse_env(t_cmd *cmd, t_env *env)
{
	int			pass;
	t_env_key	*tmp;
	char		*value;

	check_padding(cmd, env, 0);
	while (cmd)
	{
		pass = 0;
		tmp = cmd->env_key;
		while (tmp)
		{
			if (tmp->is_key == 1)
			{
				value = get_value(env, tmp->key);
				if (value)
					cmd->token = sandwich(cmd->token, value, pass, 0);
				else
					cmd->token = sandwich(cmd->token, "", pass, 0);
			}
			else if (tmp->is_key == 0)
				cmd->token = parse_env_not_key(cmd->token, tmp->key, &pass);
			tmp = tmp->next;
		}
		cmd = cmd->next;
	}
}
