/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 16:55:20 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/23 01:12:27 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../minishell.h"

static void	print_env(t_env *env)
{
	while (env)
	{
		if (env->is_env == 1)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int	do_env(char **av, t_env **env)
{
	if (av[1])
	{
		ft_putendl_fd("env: too many arguments", STDERR_FILENO);
		return (1);
	}
	print_env(*env);
	return (0);
}
