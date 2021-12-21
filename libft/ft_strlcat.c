/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 23:35:28 by kanlee            #+#    #+#             */
/*   Updated: 2021/03/13 03:29:00 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	srclen;
	size_t	dstlen;

	srclen = ft_strlen(src);
	dstlen = ft_strlen(dst);
	i = 0;
	while (src[i] && dstlen + i + 1 < size)
	{
		dst[dstlen + i] = src[i];
		i++;
	}
	if (dstlen + 1 < size)
		dst[dstlen + i] = '\0';
	if (dstlen > size)
		dstlen = size;
	return (srclen + dstlen);
}
