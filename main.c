/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 16:59:43 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/19 16:50:35 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"
#include "libft/libft.h"

int		g_exit_status = 0;



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

void	get_line(char **line)
{
	extern int rl_catch_signals;

	rl_catch_signals = 0;
	if (*line)
	{
		free(*line);
		*line = NULL;
	}
	*line = readline("$>");
	if (*line == NULL)
		error_out(" exit");
	if (ft_strncmp(*line, "", 1))
		add_history(*line);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	char	*line;
	t_cmd	*cmd;
	int		err_flag;

	(void)ac;
	(void)av;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	env = make_env(envp, NULL, NULL);
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
		if (err_flag)
			continue;
		parse_env(cmd, env);
		g_exit_status = exec_line(cmd, &env);
	}
	return (0);
}
