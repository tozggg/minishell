/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/04 19:19:29 by kanlee            #+#    #+#             */
/*   Updated: 2021/03/13 03:28:41 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*ptr;

	len = 0;
	while (s[len++])
		;
	ptr = malloc(sizeof(char) * len);
	if (!ptr)
		return (NULL);
	while (len--)
		ptr[len] = s[len];
	return (ptr);
}
