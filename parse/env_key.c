/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_key.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 20:04:16 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/26 21:15:08 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../minishell.h"

t_env_key	*init_env_key(void)
{
	t_env_key	*env_key;

	env_key = (t_env_key *)malloc(sizeof(t_env_key));
	if (!env_key)
		error_out("malloc error");
	env_key->is_key = 0;
	env_key->out_quote = 0;
	env_key->key = ft_strdup("");
	env_key->next = NULL;
	return (env_key);
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
