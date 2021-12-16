/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/04 13:01:57 by kanlee            #+#    #+#             */
/*   Updated: 2020/10/04 13:19:24 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;
	int		cmp;

	i = 0;
	while (i < n)
	{
		cmp = ((unsigned char *)s1)[i] - ((unsigned char *)s2)[i];
		if (cmp == 0)
			i++;
		else
			return (cmp);
	}
	return (0);
}
