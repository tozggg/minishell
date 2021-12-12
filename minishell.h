/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:00:27 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/12 19:23:19 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "parse/tmp_listfunc.h"

typedef struct s_pipefd {
	int	read_fd;
	int	write_fd;
}	t_pipefd;

/* exec_line.c */
int	exec_line(t_cmd *head);

/* exec_command.c */
int	command(t_cmd *node, int piperead, int pipewrite);

/* utils.c */
void	ft_lstcut(t_list *lst, t_list *el);
void	ft_lstremove(t_list **head, t_list *el);
char	**listtostrarray(t_cmd *list);

#endif
