/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 23:48:09 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/22 05:35:04 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "../minishell.h"

static int	go_home(char *str, t_env *env)
{
	if (has_env("HOME", env))
	{
		if (chdir(get_value(env, "HOME")) < 0)
		{
			perror(str);
			return (1);
		}
	}
	else
	{
		if (str && ft_strequ(str, "~"))
		{
			if (has_env("$HOME", env))
			{
				if (chdir(get_value(env, "$HOME")) < 0)
				{
					perror(str);
					return (1);
				}
				return (0);
			}
		}
		return (home_error_print());
	}
	return (0);
}

static int	cd_absolute_path(char *str)
{
	if (chdir(str) < 0)
	{
		perror(str);
		return (1);
	}
	return (0);
}

static int	cd_relative_path(char *str, t_env *env)
{
	int		res;
	char	*tmp1;

	res = 0;
	if (*str == '~')
	{
		if (!has_env("$HOME", env))
			return (home_error_print());
		if (str[1] == '/')
			tmp1 = ft_strjoin(get_value(env, "$HOME"), str + 1);
		else
			tmp1 = ft_strdup(str);
		if (chdir(tmp1) < 0)
		{
			perror(tmp1);
			res = 1;
		}
		free(tmp1);
	}
	else if (chdir(str) < 0)
	{
		perror(str);
		res = 1;
	}
	return (res);
}

int	do_cd(char **av, t_env **env)
{
	if (!av[1] || ft_strequ(av[1], "") || ft_strequ(av[1], "~"))
		return (go_home(av[1], *env));
	if (*(av[1]) == '/')
		return (cd_absolute_path(av[1]));
	else
		return (cd_relative_path(av[1], *env));
}
