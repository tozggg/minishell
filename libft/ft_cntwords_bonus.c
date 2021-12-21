/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cntwords_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 13:21:05 by kanlee            #+#    #+#             */
/*   Updated: 2021/03/13 03:19:32 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_cntwords(const char *str, char sep)
{
	int	cnt;

	cnt = 0;
	while (*str == sep)
		str++;
	while (1)
	{
		if (!(*str == sep || *str == '\0'))
		{
			str++;
			continue ;
		}
		while (*str == sep)
			str++;
		cnt++;
		if (*str == '\0')
			return (cnt);
	}
}
