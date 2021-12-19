/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 16:55:20 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/19 20:20:10 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_env(t_env *env)
{
	while (env)
	{
		if (env->is_env)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int	do_env(char **av, t_env **env)
{
	if (av[1])
	{
		errno_print(2);
		return (127);
	}
	print_env(*env);
	return (0);
}
