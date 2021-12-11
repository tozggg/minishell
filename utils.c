/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 17:02:02 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/11 18:44:09 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft/libft.h"

// terminate list at specified element
// remainders should be saved befor function call
void	ft_lstcut(t_list *lst, t_list *el)
{
	if (!lst || !el)
		return ;
	while (lst != NULL && lst->next != el)
		lst = lst->next;
	ft_lstdelone(lst->next, free);
	lst->next = NULL;
}

// remove specifed element and connect prev/next nodes
// edge cases NOT thoroughly checked !!!
void	ft_lstremove(t_list **head, t_list *el)
{
	t_list	*node;

	node = *head;
	if (*head == el)
	{
		*head = (*head)->next;
		ft_lstdelone(el, free);
		return ;
	}
	while (node != NULL && node->next != el)
		node = node->next;
	node->next = node->next->next;
	ft_lstdelone(el, free);
}

// create NULL terminated array of strings based on given list.
// can be used by execv() family.
// FIXME: need malloc guard
char	**listtostrarray(t_list *list)
{
	char	**str;
	int		n;
	int		i;

	n = ft_lstsize(list);
	str = malloc(sizeof(char *) * (n + 1));
	i = -1;
	while (++i < n)
	{
		str[i] = (char *)list->content;
		list = list->next;
	}
	str[i] = NULL;
	return (str);
}
