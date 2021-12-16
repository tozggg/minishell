/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 03:40:42 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/17 03:41:35 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft/libft.h"
#include "parse/tmp_listfunc.h"
#include <stdio.h>

void	error_out(char *str)
{
	printf("%s\n", str);
	exit(1);
}

void	err_print(int err_flag)
{
	if (err_flag == QUOTE_ERR)
		printf("Error : the number of quote is odd\n");
	if (err_flag == SYNTAX_ERR)
		printf("Erorr : invalid syntax\n");
}
