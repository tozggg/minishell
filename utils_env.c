/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:02:02 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/22 09:34:54 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "minishell.h"
#include "libft/libft.h"

static int	cnt_env(t_env *node)
{
	int	n;

	n = 0;
	while (node != NULL)
	{
		if (node->is_env == 1)
			n++;
		node = node->next;
	}
	return (n);
}

static char	*envtostr(t_env *node)
{
	char	*tmp1;
	char	*tmp2;

	if (node->is_env == 0)
		return (ft_strdup(node->key));
	else if (node->is_env == 1)
	{
		tmp1 = ft_strdup(node->key);
		tmp1 = append(tmp1, '=');
		tmp2 = ft_strjoin(tmp1, node->value);
		free(tmp1);
		return (tmp2);
	}
	return (NULL);
}

char	**envtostrarray(t_env *node)
{
	char	**str;
	int		n;
	int		i;

	n = cnt_env(node);
	str = malloc(sizeof(char *) * (n + 1));
	if (!str)
	{
		errno_print(errno, "malloc failed");
		exit(1);
	}
	i = 0;
	while (node != NULL)
	{
		if (node->is_env == 1)
			str[i++] = envtostr(node);
		node = node->next;
	}
	str[i] = NULL;
	return (str);
}

void	free_envp(char **envp)
{
	while (*envp)
	{
		free(*envp);
		envp++;
	}
	return ;
}
