/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/04 19:51:20 by kanlee            #+#    #+#             */
/*   Updated: 2021/03/13 04:03:52 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	src_len;
	size_t	dst_len;
	char	*dst;
	size_t	idx;

	if (!s)
		return (NULL);
	src_len = 0;
	while (s[src_len])
		src_len++;
	dst_len = 0;
	if (src_len >= start && src_len - start < len)
		dst_len = src_len - start;
	if (src_len >= start && src_len - start >= len)
		dst_len = len;
	dst = malloc(sizeof(char) * (dst_len + 1));
	if (!dst)
		return (NULL);
	idx = -1;
	while (++idx < dst_len)
		dst[idx] = s[start + idx];
	dst[idx] = '\0';
	return (dst);
}
