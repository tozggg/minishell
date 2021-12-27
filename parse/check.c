/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/27 18:02:53 by taejkim           #+#    #+#             */
/*   Updated: 2021/12/27 18:11:26 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redirection_node(t_cmd *node)
{
	if (node->has_quote == 1)
		return (NONE);
	if (ft_strequ(node->token, ">"))
		return (RD_WRITE);
	if (ft_strequ(node->token, ">>"))
		return (RD_APPEND);
	if (ft_strequ(node->token, "<"))
		return (RD_READ);
	if (ft_strequ(node->token, "<<"))
		return (RD_HEREDOC);
	return (NONE);
}

int	is_pipe_node(t_cmd *node)
{
	if (node->has_quote == 0 && ft_strequ(node->token, "|"))
		return (1);
	return (0);
}

int	is_heredoc_node(t_cmd *node)
{
	if (node->has_quote == 0 && ft_strequ(node->token, "<<"))
		return (1);
	return (0);
}

static int	get_near_errflag(char *str)
{
	if (ft_strequ(str, ">"))
		return (NEAR_WRITE_ERR);
	else if (ft_strequ(str, "<"))
		return (NEAR_READ_ERR);
	else if (ft_strequ(str, ">>"))
		return (NEAR_APPEND_ERR);
	else if (ft_strequ(str, "<<"))
		return (NEAR_HEREDOC_ERR);
	return (0);
}

int	check_cmd(t_cmd *curr)
{
	t_cmd	*prev;

	if (is_pipe_node(curr))
		return (NEAR_PIPE_ERR);
	prev = NULL;
	while (curr)
	{
		if (is_pipe_node(curr) && prev && is_pipe_node(prev))
			return (NEAR_PIPE_ERR);
		if (is_redirection_node(curr))
		{
			if (prev && is_redirection_node(prev))
				return (get_near_errflag(curr->token));
			if (!(curr->next))
				return (NEAR_NEWRINE_ERR);
			if (is_pipe_node(curr->next))
				return (get_near_errflag(curr->token));
		}
		if (!(curr->next) && is_pipe_node(curr))
			return (NEAR_PIPE_ERR);
		prev = curr;
		curr = curr->next;
	}
	return (0);
}
