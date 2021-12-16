/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 21:23:24 by kanlee            #+#    #+#             */
/*   Updated: 2021/03/13 03:24:50 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

static int	ft_abs(int n)
{
	if (n > 0)
		return (n);
	return (-n);
}

void		ft_putnbr_fd(int n, int fd)
{
	char	buf[20];
	int		i;
	int		is_neg;

	is_neg = (n < 0);
	i = 0;
	if (n == 0)
		buf[i++] = '0';
	while (n != 0)
	{
		buf[i++] = '0' + ft_abs(n % 10);
		n /= 10;
	}
	if (is_neg)
		buf[i++] = '-';
	while (--i >= 0)
		write(fd, &buf[i], 1);
	return ;
}
