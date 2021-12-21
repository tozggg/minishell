/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 23:30:49 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/21 15:41:42 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "../minishell.h"

int	do_pwd(void)
{
	char	*buf;

	buf = 0;
	buf = getcwd(NULL, 0);
	printf("%s\n", buf);
	free(buf);
	return (0);
}
