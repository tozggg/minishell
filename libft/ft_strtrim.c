/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 00:01:07 by kanlee            #+#    #+#             */
/*   Updated: 2021/03/13 03:32:37 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	match(char ch, char const *set)
{
	size_t	idx;

	idx = 0;
	while (set[idx])
	{
		if (set[idx] == ch)
			return (1);
		else
			idx++;
	}
	return (0);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	size_t	lpos;
	size_t	rpos;
	char	*dst;

	if (!s1 || !set)
		return (NULL);
	len = ft_strlen(s1);
	lpos = 0;
	while (match(s1[lpos], set) && lpos < len)
		lpos++;
	rpos = len - 1;
	while (match(s1[rpos], set) && rpos > 0)
		rpos--;
	if (lpos > rpos)
		return (ft_strdup(""));
	dst = malloc(sizeof(char) * (rpos - lpos + 2));
	if (!dst)
		return (NULL);
	ft_memcpy(dst, s1 + lpos, (rpos - lpos + 1));
	dst[rpos - lpos + 1] = '\0';
	return (dst);
}
