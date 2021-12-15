/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 16:59:43 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/15 00:54:55 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include "minishell.h"
#include "libft/libft.h"
#include "parse/tmp_listfunc.h"

void	err_print(int err_flag)
{
	if (err_flag == QUOTE_ERR)
		printf("Error : the number of quote is odd\n");
	if (err_flag == SYNTAX_ERR)
		printf("Erorr : invalid syntax\n");
}

void	sig_handler(int signo)
{
	pid_t	pid;
	int		wstatus;

	pid = waitpid(-1, &wstatus, WNOHANG);
	if (signo == SIGINT)
	{
		if (pid == -1)
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}	
		else
		{
			// 자식프로세스에 SIGINT? 애초에 자식 프로세스만 구별가능??
		}
	}
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_cmd	*cmd;
	int		err_flag;

	av=env=0;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	line = 0;
	cmd = 0;
	while (1)
	{
		get_line(&line);
		err_flag = 0;
		parse(&cmd, line, &err_flag);
		if (!err_flag)
			err_flag = check_cmd(cmd);
		err_print(err_flag);
#if 0
		printf("===========================\n");
		t_cmd *tmp = cmd;
		while (tmp)
		{
			printf("%s\t", tmp->token);
			t_env_key *tmp_key = tmp->env_key;
			while (tmp_key)
			{
				printf("%d:%s\t", tmp_key->is_key, tmp_key->key);
				tmp_key = tmp_key->next;
			}
			printf("\n");
			tmp = tmp->next;
		}
		printf("errflag=%d\n", err_flag);
		printf("===========================\n");
#else
		if (err_flag)
			continue;
		exec_line(cmd);
#endif
	}
	return (0);
}
