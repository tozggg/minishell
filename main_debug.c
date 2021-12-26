/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_debug.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 03:24:38 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/26 08:28:27 by kanlee           ###   ########.fr       */
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

int	get_line(char **line)
{
	char		*line_trimmed;
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
	line_trimmed = ft_strtrim(*line, " ");
	free(*line);
	*line = line_trimmed;
	if (**line == '\0')
		return (-1);
	add_history(*line);
	return (0);
}

#endif
