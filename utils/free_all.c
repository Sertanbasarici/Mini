/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasari <sebasari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 10:47:31 by sebasari          #+#    #+#             */
/*   Updated: 2024/09/27 13:58:19 by sebasari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_minishell	g_minishell;

void	free_token(t_list *token)
{
	t_list *tmp;

	tmp = token;
	while (tmp != NULL)
	{
		ft_lstdelone(tmp, del);
		token = token->next;
		tmp = token;
		printf("gecti token \n");
	}
}

void	free_file(t_file *file)
{
	free(file->after);
	free(file);
}

void	free_parse(t_parse *parse)
{
	t_parse	*tmp;
	int		i;

	i = 0;
	tmp = parse;
	while (tmp != NULL)
	{
		while (tmp->args[i] != NULL)
			free(tmp->args[i++]);
		free(tmp->args);
		if (tmp->file != NULL)
			free_file(tmp->file);
		free(tmp);
		parse = parse->next;
		tmp = parse;
	}
}

void	free_fd(t_fd *fd)
{
	t_fd *tmp;

	tmp = fd;
	while (tmp != NULL)
	{
		free(tmp);
		fd = fd->next;
		tmp = fd;
		printf("gecti fd\n");
	}
}

void	free_all(char *str)
{
	if (str)
	{
		printf("gecti 1\n");
		free(str);
	}
	if (g_minishell.nodes_t)
	{
		printf("gecti 2\n");
		free_token(g_minishell.nodes_t);
	}
	if (g_minishell.nodes_p)
	{
		printf("gecti 3\n");
		free_parse(g_minishell.nodes_p);
	}
	if (g_minishell.fd)
	{
		printf("gecti 4\n");
		free_fd(g_minishell.fd);
	}
}