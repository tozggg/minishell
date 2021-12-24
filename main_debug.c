/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_debug.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 03:24:38 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/24 03:24:47 by kanlee           ###   ########.fr       */
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

#ifdef DEBUG

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
			printf("[%d]", g_exit_status);
			rl_redisplay();
		}	
	}
}

void	get_line(char **line)
{
	extern int	rl_catch_signals;

	rl_catch_signals = 0;
	if (*line)
	{
		free(*line);
		*line = NULL;
	}
	printf("[%d]", g_exit_status);
	*line = readline("minimini-shell $ ");
	if (*line == NULL)
	{
		ft_putendl_fd("exit", STDERR_FILENO);
		exit(g_exit_status);
	}
	if (ft_strncmp(*line, "", 1))
		add_history(*line);
}

#endif
