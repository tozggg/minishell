/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 23:48:09 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/23 01:09:53 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "../minishell.h"

static int	go_home(char *str, t_env *env)
{
	if (has_env("HOME", env))
	{
		if (chdir(get_value(env, "HOME")))
		{
			errno_print(errno, str);
			return (1);
		}
	}
	else
	{
		if (str && ft_strequ(str, "~"))
		{
			if (has_env("$HOME", env))
			{
				if (chdir(get_value(env, "$HOME")))
				{
					errno_print(errno, str);
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
	if (chdir(str))
	{
		errno_print(errno, str);
		return (1);
	}
	return (0);
}

static int	cd_relative_path(char *str, t_env *env)
{
	int		res;
	char	*tmp;

	res = 0;
	if (*str == '~')
	{
		if (!has_env("$HOME", env))
			return (home_error_print());
		if (str[1] == '/')
			tmp = ft_strjoin(get_value(env, "$HOME"), str + 1);
		else
			tmp = ft_strdup(str);
		if (chdir(tmp) < 0)
		{
			errno_print(errno, str);
			res = 1;
		}
		free(tmp);
	}
	else if (chdir(str) < 0)
	{
		errno_print(errno, str);
		res = 1;
	}
	return (res);
}

void	env_pwd_update(t_env *env)
{
	char	*oldpwd;
	char	*pwd;

	if (!has_env("PWD", env))
		return ;
	oldpwd = ft_strdup(get_value(env, "PWD"));
	pwd = getcwd(NULL, 0);
	modify_env("PWD", pwd, 0, env);
	free(pwd);
	if (has_env("OLDPWD", env))
		modify_env("OLDPWD", oldpwd, 0, env);
	free(oldpwd);
}

int	do_cd(char **av, t_env **env)
{
	int	res;

	if (!av[1] || ft_strequ(av[1], "") || ft_strequ(av[1], "~"))
		res = go_home(av[1], *env);
	else if (*(av[1]) == '/')
		res = cd_absolute_path(av[1]);
	else
		res = cd_relative_path(av[1], *env);
	if (res == 0)
		env_pwd_update(*env);
	return (res);
}
