/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 16:59:43 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/14 15:41:19 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft/libft.h"
#include "parse/tmp_listfunc.h"

int	main(void)
{
#if 0
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
#else
	t_cmd	*head;
	t_cmd	*node;
	head = 0;
	node = init_cmd(); node->token = ft_strdup("cat"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup("<<"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup("EOF"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup(">"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup("out1"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup("|"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup("echo"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup("asdf"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup(">"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup("out2"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup("|"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup("cat"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup("<<"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup("EEE"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup(">"); add_cmd(&head, node);
	node = init_cmd(); node->token = ft_strdup("out3"); add_cmd(&head, node);

	printf("input: ");
	node = head;
	while (node != NULL)
	{
		printf("%s ", node->token);
		node = node->next;
	}
#endif
	printf("\n\n\n");
	exec_line(head);
	destory_cmd(&head); //FIXME: heap-use-after-free
	printf("\nreturn to prompt\n");
	return (0);
}

