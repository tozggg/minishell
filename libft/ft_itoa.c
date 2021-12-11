/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 00:44:20 by kanlee            #+#    #+#             */
/*   Updated: 2021/03/13 03:20:41 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static char	*i_to_a(char *ret, int n, int len)
{
	int	sign;
	int	i;

	sign = 1;
	if (n < 0)
		sign = -1;
	ret[len] = '\0';
	i = 0;
	if (n < 0)
		ret[i++] = '-';
	while (i < len)
	{
		if (sign > 0)
			ret[len - i - 1] = '0' + n % 10;
		else
			ret[len - i] = '0' + (n % 10) * -1;
		n /= 10;
		i++;
	}
	return (ret);
}

char		*ft_itoa(int n)
{
	int		len;
	char	*ret;
	int		n_tmp;

	n_tmp = n;
	len = 0;
	if (n <= 0)
		len++;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (NULL);
	return (i_to_a(ret, n_tmp, len));
}
