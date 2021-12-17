/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvpe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 07:37:04 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/16 22:25:11 by kanlee           ###   ########.fr       */
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
static char	**get_pathlist(void)
{
	char		*pathenv;
	char		**pathlist;
	char		*tmp;
	char		**ret;

	pathenv = getenv("PATH");
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

static int	is_path(char *str)
{
	while (*str)
	{
		if (*str == '/')
			return (1);
		str++;
	}
	return (0);
}

static int	exec_path(char *cmd, char **arg, char **env)
{
	if (is_dir(cmd))
	{
		errno = EISDIR;
		return (-1);
	}
	return (execve(cmd, arg, env));
}

/* if cmd if builtin, execute exec_builtin()
 * if cmd points to absolute or relative path, execute it
 * if $PATH_1/cmd exists and executable, execute it
 * if $PATH_2/cmd exists and executable, execute it
 * if anyting exists or executable, return FILE_NOT_FOUND or PERMISSION_DENIED
*/
int	ft_execvpe(char *cmd, char **arg, char **env)
{
	char	**pathlist;
	char	**pathlist_bak;
	char	*tmp;

	if (is_builtin(cmd))
		return (exec_builtin(arg));
	// if absolute or relative path, execute exact target.
	if (is_path(cmd))
		return (exec_path(cmd, arg, env));
	pathlist = get_pathlist();
	pathlist_bak = pathlist;
	while (pathlist && *pathlist)
	{
		tmp = ft_strjoin(*pathlist, cmd);
		execve(tmp, arg, env);
		free(tmp);
		free(*pathlist);
		pathlist++;
		if (errno == EACCES)
			return (-1);
	}
	free(pathlist_bak);
	return (-1);
}
