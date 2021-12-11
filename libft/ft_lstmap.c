/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/08 14:06:18 by kanlee            #+#    #+#             */
/*   Updated: 2021/04/16 19:13:00 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_first;
	t_list	*tmp;
	t_list	*prev;

	if (!lst || !f)
		return (NULL);
	new_first = ft_lstnew((*f)(lst->content));
	if (!new_first)
		return (NULL);
	prev = new_first;
	tmp = lst->next;
	while (tmp)
	{
		prev->next = ft_lstnew((*f)(tmp->content));
		if (!(prev->next))
		{
			ft_lstclear(&new_first, del);
			return (NULL);
		}
		prev = prev->next;
		tmp = tmp->next;
	}
	return (new_first);
}
