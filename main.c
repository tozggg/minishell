/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 16:59:43 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/11 17:33:01 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft/libft.h"

int	main(void)
{
	t_list	*head;
	t_list	*node;

	head = 0;
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("ls")));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup(">")));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("outfile")));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("-al")));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("|")));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("tail")));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("-n")));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("4")));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("|")));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("sort")));
	printf("input: ");
	node = head;
	while (node != NULL)
	{
		char *data = (char *)node->content;
		printf("%s ", data);
		node = node->next;
	}
	printf("\n\n\n");
	exec_line(head);
	printf("\nreturn to prompt\n");
	return (0);
}

