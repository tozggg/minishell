/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:59:27 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/14 20:58:21 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <termios.h>
#include <term.h>


#define SYNTAX_ERR 1
#define QUOTE_ERR 2
#define EMPTY_LINE 3

// 명령어 token으로 나눈 linkedlist 구조체
typedef struct	s_cmd
{
	char				*token;
	struct s_env_key	*env_key;
	struct s_cmd		*next;
}	t_cmd;

// 환경변수 $가 치환되는 key값의 linkedlist
typedef struct	s_env_key
{
	int					is_key;
	char				*key;
	struct s_env_key	*next;
}	t_env_key;








void	error_out(char *str)
{
	printf("%s\n", str);
	exit(1);
}

// Libft-------------------------------------------------------------------------

size_t		ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

char	*ft_strdup(const char *s1)
{
	char	*ptr;
	size_t	len;
	size_t	i;

	len = ft_strlen(s1);
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	if (!ptr)
		error_out("malloc error");
	i = 0;
	while (i < len)
	{
		ptr[i] = s1[i];
		++i;
	}
	ptr[i] = 0;
	return (ptr);
}

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	const unsigned char *s1_u;
	const unsigned char *s2_u;
	size_t				i;

	s1_u = (unsigned char *)s1;
	s2_u = (unsigned char *)s2;
	i = 0;
	while (s1_u[i] && s2_u[i] && (i < n))
	{
		if (s1_u[i] != s2_u[i])
			return (s1_u[i] - s2_u[i]);
		++i;
	}
	if (i == n)
		return (0);
	if (s1_u[i] != s2_u[i])
		return (s1_u[i] - s2_u[i]);
	else
		return (0);
}	

int	ft_strequ(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	if (s1[i] || s2[i])
		return (0);
	return (1);
}

//-------------------------------------------------------------------------


char	*append(char *str, char c)
{
	char	*res;
	int	len;
	int	i;

	len = ft_strlen(str) + 1;
	res = (char *)malloc(sizeof(char) * (len + 1));
	i = 0;
	while (i < len - 1)
	{
		res[i] = str[i];
		++i;
	}
	res[i++] = c;
	res[i] = 0;
	free(str);
	return (res);
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
		error_out("exit");
	add_history(*line);
}

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		error_out("malloc error");
	cmd->token = ft_strdup("");
	cmd->env_key = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	destroy_cmd(t_cmd **ptr)
{
	t_cmd		*cmd;
	t_cmd		*tmp_c;
	t_env_key	*tmp_e;

	cmd = *ptr;
	while (cmd)
	{
		tmp_c = cmd->next;
		while (cmd->env_key)
		{
			tmp_e = cmd->env_key->next;
			free(cmd->env_key->key);
			free(cmd->env_key);
			cmd->env_key = tmp_e;
		}
		free(cmd->token);
		free(cmd);
		cmd = tmp_c;
	}
	*ptr = NULL;
}

void	add_cmd(t_cmd **ptr, t_cmd *cmd)
{
	t_cmd	*tmp;

	if (*ptr == NULL)
		*ptr = cmd;
	else
	{
		tmp = *ptr;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = cmd;
	}
}

void	add_env_key(t_cmd *cmd, t_env_key *env_key)
{
	t_env_key	*tmp;

	if (cmd->env_key == NULL)
		cmd->env_key = env_key;
	else
	{
		tmp = cmd->env_key;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = env_key;
	}
}

t_env_key	*init_env_key(void)
{
	t_env_key	*env_key;

	env_key = (t_env_key *)malloc(sizeof(t_env_key));
	if (!env_key)
		error_out("malloc error");
	env_key->is_key = 0;
	env_key->key = ft_strdup("");
	env_key->next = NULL;
	return (env_key);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	is_separator(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

int	is_allow_envpname(char c)
{
	if ('0' <= c && c <= '9')
		return (1);
	if ('a' <= c && c <= 'z')
		return (1);
	if ('A' <= c && c <= 'Z')
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

int	is_unspecified_char(char c)
{
	if (c == '\\' || c == ';' || c == '(' \
		|| c == ')' || c == '*' || c == '&' || c == '#')
		return (1);
	return (0);
}

char	*separate_pipe(t_cmd **ptr, t_cmd *cmd, char *line)
{
	t_cmd	*tmp;

	if (!ft_strncmp(cmd->token, "", 1))
	{
		cmd->token = append(cmd->token, '|');
		add_cmd(ptr, cmd);
	}
	else
	{
		add_cmd(ptr, cmd);
		tmp = init_cmd();
		tmp->token = append(tmp->token, '|');
		add_cmd(ptr, tmp);
	}
	return (line + 1);
}

char	*separate_rdr_out(t_cmd **ptr, t_cmd *cmd, char *line)
{
	t_cmd	*tmp;

	if (!ft_strncmp(cmd->token, "", 1))
	{
		cmd->token = append(cmd->token, '>');
		if (!ft_strncmp(line, ">>", 2))
			cmd->token = append(cmd->token, '>');
		add_cmd(ptr, cmd);
	}
	else
	{
		add_cmd(ptr, cmd);
		tmp = init_cmd();
		tmp->token = append(tmp->token, '>');
		if (!ft_strncmp(line, ">>", 2))
			tmp->token = append(tmp->token, '>');
		add_cmd(ptr, tmp);
	}
	if (!ft_strncmp(line, ">>", 2))
		return (line + 2);
	return (line + 1);
}

char	*separate_rdr_in(t_cmd **ptr, t_cmd *cmd, char *line)
{
	t_cmd	*tmp;

	if (!ft_strncmp(cmd->token, "", 1))
	{
		cmd->token = append(cmd->token, '<');
		if (!ft_strncmp(line, "<<", 2))
			cmd->token = append(cmd->token, '<');
		add_cmd(ptr, cmd);
	}
	else
	{
		add_cmd(ptr, cmd);
		tmp = init_cmd();
		tmp->token = append(tmp->token, '<');
		if (!ft_strncmp(line, "<<", 2))
			tmp->token = append(tmp->token, '<');
		add_cmd(ptr, tmp);
	}
	if (!ft_strncmp(line, "<<", 2))
		return (line + 2);
	return (line + 1);
}

char	*separate(t_cmd **ptr, t_cmd **cmd, char *line)
{
	if (is_space(*line))
		add_cmd(ptr, *cmd);
	else if (*line == '|')
		line = separate_pipe(ptr, *cmd, line);
	else if (*line == '>')
		line = separate_rdr_out(ptr, *cmd, line);
	else if (*line == '<')
		line = separate_rdr_in(ptr, *cmd, line);
	while (*line && is_space(*line))
		++line;
	if (*line)
		*cmd = init_cmd();
	else
		*cmd = NULL;
	return (line);
}

char	*envp_decision(t_cmd *cmd, char *line)
{
	t_env_key	*env_key;
	
	env_key = init_env_key();
	++line;
	if (!(*line) || *line == ' ' || *line == '\"')
	{
		env_key->key = append(env_key->key, '$');
		add_env_key(cmd, env_key);
	}
	else if (*line == '\'')
		add_env_key(cmd, env_key);
	else
	{
		env_key->is_key = 1;
		while (*line && is_allow_envpname(*line))
			env_key->key = append(env_key->key, *(line++));
		add_env_key(cmd, env_key);
	}
	cmd->token = append(cmd->token, '$');
	return (line);
}

char	*small_quote_envp_decision(t_cmd *cmd, char *line)
{
	t_env_key	*env_key;
	
	env_key = init_env_key();
	env_key->key = append(env_key->key, '$');
	add_env_key(cmd, env_key);
	cmd->token = append(cmd->token, '$');
	return (line + 1);
}

char	*big_quote_envp_decision(t_cmd *cmd, char *line)
{
	t_env_key	*env_key;
	
	env_key = init_env_key();
	++line;
	if (!(*line) || *line == ' ' || *line == '\'' || *line == '\"')
	{
		env_key->key = append(env_key->key, '$');
		add_env_key(cmd, env_key);
	}
	else
	{
		env_key->is_key = 1;
		while (*line && is_allow_envpname(*line))
			env_key->key = append(env_key->key, *(line++));
		add_env_key(cmd, env_key);
	}
	cmd->token = append(cmd->token, '$');
	return (line);
}

char	*start_quote(t_cmd *cmd, char *line, int *err_flag)
{
	char quote;

	quote = *line;
	++line;
	while (*line)
	{
		if (*line == quote)
			return (line + 1);
		if (*line == '$')
		{
			if (quote == '\"')
				line = big_quote_envp_decision(cmd, line);
			else
				line = small_quote_envp_decision(cmd, line);
		}
		else
			cmd->token = append(cmd->token, *(line++));
	}
	*err_flag = QUOTE_ERR;
	return (line);
}

void	parse(t_cmd **ptr, char *line, int *err_flag)
{
	t_cmd	*cmd;
	
	if (*ptr)
		destroy_cmd(ptr);
	if (!(*line))
		*err_flag = EMPTY_LINE;
	cmd = init_cmd();
	while (*line && is_space(*line))
		++line;
	while (*line)
	{
		if (is_unspecified_char(*line))
			*err_flag = SYNTAX_ERR;
		if (is_space(*line) || is_separator(*line))
			line = separate(ptr, &cmd, line);
		else if (*line == '\"' || *line == '\'')
			line = start_quote(cmd, line, err_flag);
		else if (*line == '$')
			line = envp_decision(cmd, line);
		else
			cmd->token = append(cmd->token, *(line++));
	}
	add_cmd(ptr, cmd);
}

int	check_cmd(t_cmd *cmd)
{
	int	prev_token_is_pipe;
	int	prev_token_is_rdr;

	if (ft_strequ(cmd->token, "|"))
		return (SYNTAX_ERR);
	prev_token_is_pipe = 0;
	prev_token_is_rdr = 0;
	while (cmd)
	{
		if ((prev_token_is_pipe || prev_token_is_rdr) && ft_strequ(cmd->token, "|"))
			return (SYNTAX_ERR);
		if (ft_strequ(cmd->token, "|"))
			prev_token_is_pipe = 1;
		else
			prev_token_is_pipe = 0;
		if (ft_strequ(cmd->token, "<") || ft_strequ(cmd->token, ">") ||\
				ft_strequ(cmd->token, "<<") || ft_strequ(cmd->token, ">>"))
			prev_token_is_rdr = 1;
		else
			prev_token_is_rdr = 0;
		if (!(cmd->next) && ft_strequ(cmd->token, "|"))
			return (SYNTAX_ERR);
		cmd = cmd->next;
	}
	return (0);
}

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



int	main(int ac, char *av[], char *envp[])
{
	char	*line;
	t_cmd	*cmd;
	int		err_flag;

	(void)ac;
	(void)av;
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

	}
	return (0);
}
