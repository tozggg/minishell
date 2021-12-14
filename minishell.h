/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:00:27 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/13 19:28:18 by kanlee           ###   ########.fr       */
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

typedef struct s_rdinfo {
	int read_fd;
	int write_fd;
}	t_rdinfo;

enum e_rdtype {
	NONE = 0,
	RD_WRITE,
	RD_APPEND,
	RD_READ,
	RD_HEREDOC
};

/* exec_line.c */
int	exec_line(t_cmd *head);

/* exec_command.c */
int	command(t_cmd *node, t_pipefd pipefd);

/* redirection.c */
int	store_rdinfo(t_cmd *node, t_rdinfo *rd, int rdtype);
int	read_heredoc(t_cmd *node);

/* utils.c */
void	ft_lstcut(t_list *lst, t_list *el);
void	ft_lstremove(t_list **head, t_list *el);
char	**listtostrarray(t_cmd *list);

#endif
