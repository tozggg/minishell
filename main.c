/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 16:59:43 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/23 19:42:16 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"
#include "libft/libft.h"

int		g_exit_status = 0;

void	sig_handler(int signo)
{
	int	wstatus;

	if (signo == SIGINT)
	{
		if (waitpid(-1, &wstatus, WNOHANG) == -1 && errno == ECHILD)
		{
			g_exit_status = 1;
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
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
	{
		ft_putendl_fd("exit", STDERR_FILENO);
		exit(0);
	}
	if (ft_strncmp(*line, "", 1))
		add_history(*line);
}

static void	inc_shlvl(t_env *env)
{
	int		oldlvl;
	char	*newlvl;

	oldlvl = ft_atoi(get_value(env, "SHLVL"));
	newlvl = ft_itoa(oldlvl + 1);
	modify_env("SHLVL", newlvl, 1, env);
	free(newlvl);
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
	env = make_env(envp);
	inc_shlvl(env);
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
		g_exit_status = exec_line(cmd, &env, g_exit_status);
	}
	return (0);
}
