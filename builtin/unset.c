/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 14:50:58 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/22 19:29:15 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../minishell.h"

static int	is_valid_unset_str(char *str)
{
	int	start_not_number;
	int	i;

	start_not_number = 0;
	i = 0;
	while (str[i])
	{	
		if (!is_allow_envpname(str[i]))
			return (0);
		if (!ft_isdigit(str[i]))
			start_not_number = 1;
		else if (!start_not_number)
			return (0);
		++i;
	}
	return (1);
}

static void	free_env(char *key, char *value, t_env *env)
{
	if (key)
		free(key);
	if (value)
		free(value);
	if (env)
		free(env);
}

static void	del_env(char *key, t_env **ptr, t_env *prev, t_env *curr)
{
	while (curr)
	{
		if (ft_strequ(key, curr->key))
		{
			if (prev)
			{
				prev->next = curr->next;
				free_env(curr->key, curr->value, curr);
				curr = prev->next;
			}
			else
			{
				*ptr = curr->next;
				free_env(curr->key, curr->value, curr);
				curr = *ptr;
			}
			continue ;
		}
		prev = curr;
		curr = curr->next;
	}
}

static int	unset_str(char *str, t_env **env)
{
	if (!is_valid_unset_str(str))
		return (1);
	if (ft_strequ(str, "PWD") || ft_strequ(str, "OLDPWD"))
		modify_env(str, NULL, -1, *env);
	else
		del_env(str, env, NULL, *env);
	return (0);
}

int	do_unset(char **av, t_env **env)
{
	int	err;

	err = 0;
	++av;
	while (*av)
	{
		if (unset_str(*av, env))
		{
			identifier_err_print(*av);
			err = 1;
		}
		++av;
	}
	if (err)
		return (1);
	return (0);
}
