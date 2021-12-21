/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 09:22:51 by kanlee            #+#    #+#             */
/*   Updated: 2021/03/13 03:27:28 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int		free_all(char **ret, size_t cnt)
{
	while (cnt)
		free(ret[--cnt]);
	return (0);
}

static size_t	cnt_word(const char *s, char c)
{
	size_t	cnt;
	size_t	from;
	size_t	i;

	cnt = 0;
	i = 0;
	from = 0;
	while (1)
	{
		if ((s[i] == c || s[i] == '\0'))
		{
			if (i > from)
				cnt++;
			if (s[i] == '\0')
				return (cnt);
			from = i + 1;
		}
		i++;
	}
	return (cnt);
}

static int		ft_split_str(char const *s, char c, char **ret)
{
	size_t	cnt;
	size_t	from;
	size_t	to;

	cnt = 0;
	from = 0;
	to = 0;
	while (1)
	{
		if (s[to] == c || s[to] == '\0')
		{
			if (to - from > 0)
			{
				ret[cnt] = ft_substr(s, from, to - from);
				if (!(ret[cnt++]))
					return (free_all(ret, --cnt));
			}
			if (s[to] == '\0')
				break ;
			from = to + 1;
		}
		to++;
	}
	ret[cnt] = NULL;
	return (1);
}

char			**ft_split(char const *s, char c)
{
	size_t	cnt;
	char	**ret;

	if (!s)
		return (NULL);
	cnt = cnt_word(s, c);
	ret = malloc(sizeof(char *) * (cnt + 1));
	if (!ret)
		return (NULL);
	if (ft_split_str(s, c, ret))
		return (ret);
	else
	{
		free(ret);
		return (NULL);
	}
}
