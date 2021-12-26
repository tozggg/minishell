/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/26 23:19:04 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/26 23:21:44 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../minishell.h"

static int	has_padding(char *key, t_env *env)
{
	char	*value;

	value = NULL;
	while (env)
	{
		if (ft_strequ(key, env->key))
		{
			value = env->value;
			break ;
		}
		env = env->next;
	}
	if (!value)
		return (0);
	if (*value == ' ')
		return (1);
	while (*(value + 1))
		++value;
	if (*value == ' ')
		return (1);
	return (0);
}

static char	*cut_pre_padding(char *str, int pd)
{
	char	*res;
	int		len;
	int		i;
	int		j;

	i = 0;
	while (str[i] && str[i] == ' ')
		++i;
	while (i > 0 && --pd >= 0)
		--i;
	len = ft_strlen(&str[i]);
	res = (char *)malloc(sizeof(char) * (len + 1));
	j = 0;
	while (j < len)
		res[j++] = str[i++];
	res[j] = 0;
	return (res);
}

static char	*cut_suf_padding(char *str, int pd)
{
	char	*res;
	int		len;
	int		i;

	i = 0;
	while (str[i])
		++i;
	while (i > 0 && str[i - 1] == ' ')
		--i;
	while (str[i] && --pd >= 0)
		++i;
	len = i;
	res = (char *)malloc(sizeof(char) * (len + 1));
	i = -1;
	while (++i < len)
		res[i] = str[i];
	res[i] = 0;
	return (res);
}

static int	cut_padding(t_env_key *node, t_env *env, int pre, char suf)
{
	int		res;
	char	*tmp;
	char	*str;
	char	*value;

	value = get_value(env, node->key);
	tmp = cut_pre_padding(value, pre);
	if (!suf || suf == ' ')
	{
		str = cut_suf_padding(tmp, 0);
		res = 1;
	}
	else
	{
		str = cut_suf_padding(tmp, 1);
		res = 0;
	}
	free(tmp);
	node->is_key = 0;
	free(node->key);
	node->key = str;
	return (res);
}

void	check_padding(t_cmd *cmd, t_env *env, int pre)
{
	t_env_key	*tmp;
	int			i;

	while (cmd)
	{
		pre = 0;
		tmp = cmd->env_key;
		i = -1;
		while ((cmd->token)[++i])
		{
			if ((cmd->token)[i] == '$')
			{
				if (tmp->is_key == 1 && tmp->out_quote == 1 \
						&& has_padding(tmp->key, env))
					pre = cut_padding(tmp, env, pre, cmd->token[i + 1]);
				tmp = tmp->next;
				continue ;
			}
			if (cmd->token[i] == ' ')
				pre = 0;
			else
				pre = 1;
		}
		cmd = cmd->next;
	}
}
