/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/04 15:01:50 by kanlee            #+#    #+#             */
/*   Updated: 2021/03/13 03:30:26 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *hay, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (*needle == 0)
		return ((char *)hay);
	i = 0;
	while (*hay && i < len)
	{
		if (*hay == *needle)
		{
			j = 0;
			while (*(hay + j) && *(needle + j) && (*(hay + j) == *(needle + j))
				&& (i + j < len))
				j++;
			if (*(needle + j) == 0)
				return ((char *)hay);
		}
		hay++;
		i++;
	}
	return (0);
}
