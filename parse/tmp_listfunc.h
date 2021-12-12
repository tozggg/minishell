/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_listfunc.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:59:27 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/12 18:47:00 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TMP_LISTFUNC_H
# define TMP_LISTFUNC_H

enum e_cmdtype {
	TYPE_GENERAL = 0,
	TYPE_RDSIGN,
	TYPE_RDTARGET,
	TYPE_PIPE
};

// 명령어 token으로 나눈 linkedlist 구조체
typedef struct	s_cmd
{
	char				*token;
	int					cmd_type;
	int					cmd_end;
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

//-------------------------------------------------------------------------
t_cmd	*init_cmd(void);
void	destory_cmd(t_cmd **ptr);
void	add_cmd(t_cmd **ptr, t_cmd *cmd);
void	add_env_key(t_cmd *cmd, t_env_key *env_key);
t_env_key	*init_env_key(void);

#endif
