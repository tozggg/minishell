/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvpe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 07:37:04 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/22 09:29:10 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "minishell.h"
#include "libft/libft.h"

static int	is_dir(char *str)
{
	struct stat	buf;

	stat(str, &buf);
	if (S_ISDIR(buf.st_mode))
		return (1);
	return (0);
}

/* split $PATH to string array.
 * append '/' to every items
*/
static char	**get_pathlist(t_env *env)
{
	char		*pathenv;
	char		**pathlist;
	char		*tmp;
	char		**ret;

	pathenv = get_value(env, "PATH");
	pathlist = ft_split(pathenv, ':');
	ret = pathlist;
	while (pathlist && *pathlist)
	{
		if (is_dir(*pathlist))
		{
			tmp = ft_strjoin(*pathlist, "/");
			free(*pathlist);
			*pathlist = tmp;
		}
		pathlist++;
	}
	return (ret);
}

static void	exec_path(char *cmd, char **arg, char **env)
{
	if (is_dir(cmd))
	{
		errno_print(EISDIR, cmd);
		exit(126);
	}
	execve(cmd, arg, env);
	errno_print(errno, cmd);
	if (errno == ENOENT)
		exit(127);
	if (errno == EACCES)
		exit(126);
	exit(126);
}

static void	try_in_path(char **pathlist, char *cmd, char **arg, t_env **env)
{
	char	*target;

	while (pathlist && *pathlist)
	{
		target = ft_strjoin(*pathlist, cmd);
		free(*pathlist);
		pathlist++;
		if (is_dir(target))
		{
			free(target);
			continue ;
		}
		execve(target, arg, (char **){NULL});    // TODO: t_env to NULL terminated char**
		if (errno != ENOENT)
		{
			errno_print(errno, target);
			exit(126);
		}
		free(target);
	}
}

/* if cmd if builtin, execute exec_builtin()
 * if cmd points to absolute or relative path, execute it
 * if $PATH_1/cmd exists and executable, execute it
 * if $PATH_2/cmd exists and executable, execute it
 * if anyting exists or executable, return FILE_NOT_FOUND or PERMISSION_DENIED
*/
int	ft_execvpe(char *cmd, char **arg, t_env **env)
{
	char	**pathlist;

	if (is_builtin(cmd))
		exit(exec_builtin(arg, env));
	// if cmd has path sign, execute exat target.
	if (ft_strchr(cmd, '/') != NULL)
		exec_path(cmd, arg, (char **){NULL}); // TODO: t_env to char**
	pathlist = get_pathlist(*env);
	try_in_path(pathlist, cmd, arg, env);
	free(pathlist);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	exit(127);
}
