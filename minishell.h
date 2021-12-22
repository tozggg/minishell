/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:00:27 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/23 00:06:10 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"

extern int	g_exit_status;

enum e_errflag {
	EMPTY_LINE = 1,
	UNSPEC_CHAR_ERR,
	QUOTE_ERR,
	NEAR_PIPE_ERR,
	NEAR_WRITE_ERR,
	NEAR_READ_ERR,
	NEAR_APPEND_ERR,
	NEAR_HEREDOC_ERR,
	NEAR_NEWRINE_ERR
};

enum e_cmdtype {
	TYPE_GENERAL = 0,
	TYPE_RDSIGN,
	TYPE_RDTARGET,
	TYPE_PIPE,
	TYPE_INVALID
};

enum e_rdtype {
	NONE = 0,
	RD_WRITE,
	RD_APPEND,
	RD_READ,
	RD_HEREDOC
};

typedef struct s_cmd
{
	char				*token;
	int					cmd_type;
	int					cmd_end;
	struct s_env_key	*env_key;
	struct s_cmd		*next;
}	t_cmd;

typedef struct s_env_key
{
	int					is_key;
	char				*key;
	struct s_env_key	*next;
}	t_env_key;

typedef struct s_env
{
	int				is_env;
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_pipeinfo {
	int	read;
	int	write;
	int	unused;
}	t_pipeinfo;

typedef struct s_rdinfo {
	int	read;
	int	write;
}	t_rdinfo;

void		sig_handler(int signo);
void		get_line(char **line);

/* cmd.c */
t_cmd		*init_cmd(void);
void		add_cmd(t_cmd **ptr, t_cmd *cmd);
void		destroy_cmd(t_cmd **ptr);
int			check_cmd(t_cmd *cmd);

/* env_key.c */
t_env_key	*init_env_key(void);
void		add_env_key(t_cmd *cmd, t_env_key *env_key);

/* env.c */
char	*get_value(t_env *env, char *key);
int		has_env(char *key, t_env *env);
void	add_env(char *key, char *value, t_env **env, int is_env);
void	modify_env(char *key, char *value, int is_env, t_env *env);
t_env	*make_env(char **envp);

/* separate.c */
char		*separate(t_cmd **ptr, t_cmd **cmd, char *line);

/* parse.c */
void		parse(t_cmd **ptr, char *line, int *err_flag);

/* parse_env.c */
void		parse_env(t_cmd *cmd, t_env *env);

/* parse_utils.c */
char		*append(char *str, char c);
int			is_space(char c);
int			is_separator(char c);
int			is_allow_envpname(char c);
int			is_unspecified_char(char c);

/* exec_line.c */
int			exec_line(t_cmd *head, t_env **env, int exitcode);

/* exec_command.c */
int			command(t_cmd *node, t_pipeinfo pipeinfo, t_env **env);

/* ft_execvpe.c */
int			ft_execvpe(char *cmd, char **arg, t_env **env);

/* redirection.c */
void		chk_rdtarget(t_cmd *node);
int			is_redirection_node(t_cmd *node);
int			store_rdinfo(t_cmd *node, t_rdinfo *rd, int rdtype);
int			read_heredoc(t_cmd *node, char *limit);

/* utils.c */
char		**listtostrarray(t_cmd *list);
char		**envtostrarray(t_env *node);
void		free_envp(char **envp);
t_cmd		*has_pipe(t_cmd *node);
t_cmd		*has_heredoc(t_cmd *node);
void		safe_close_readend(int fd);

/* error.c */
void		error_out(char *str);
void		err_print(int err_flag);
void		errno_print(int errnum, char *place);
void		identifier_err_print(char *place);
int			home_error_print(void);

/* builtin/builtin.c */
int			is_builtin(char *cmd);
int			exec_builtin_single(char **av, t_rdinfo rd, t_env **env);
int			exec_builtin(char **av, t_env **env);

/* buitin */
int			do_echo(int ac, char **av);
int			do_export(char **av, t_env **env);
int			do_unset(char **av, t_env **env);
int			do_env(char **av, t_env **env);
int			do_exit(int ac, char **av);
int			do_pwd(void);
int			do_cd(char **av, t_env **env);

#endif
