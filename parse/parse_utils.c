/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 20:05:18 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/19 16:45:07 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../minishell.h"

char	*append(char *str, char c)
{
	char	*res;
	int		len;
	int		i;

	len = ft_strlen(str) + 1;
	res = (char *)malloc(sizeof(char) * (len + 1));
	i = 0;
	while (i < len - 1)
	{
		res[i] = str[i];
		++i;
	}
	res[i++] = c;
	res[i] = 0;
	free(str);
	return (res);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	is_separator(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

int	is_allow_envpname(char c)
{
	if ('0' <= c && c <= '9')
		return (1);
	if ('a' <= c && c <= 'z')
		return (1);
	if ('A' <= c && c <= 'Z')
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

int	is_unspecified_char(char c)
{
	if (c == '\\' || c == ';' || c == '(' \
		|| c == ')' || c == '*' || c == '&' || c == '#')
		return (1);
	return (0);
}
