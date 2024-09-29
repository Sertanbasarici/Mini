/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_special.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasari <sebasari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 15:07:59 by sebasari          #+#    #+#             */
/*   Updated: 2024/09/29 20:20:53 by sebasari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_special_type(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '<' || input[i] == '>' || input[i] == '|')
		{
			if ((input[i] == '>' && input[i + 1] == '>')
				|| (input[i] == '<' && input[i + 1] == '<' ))
				return (1);
			else
				return (1);
		}
		i++;
	}
	return (0);
}

t_minishell	*divide_accordingly(char *input, t_minishell *mini, int *index_num)
{
	int		i;
	int		len;
	int		start;

	start = 0;
	len = 0;
	i = 0;
	while (input[i])
	{
		len = 0;
		if (!ft_special_type_index(input[i]))
		{
			i = ft_not_q(input, i);
			len = i - len;
			mini = ft_add_new_node(start, len, mini, index_num);
			start = i;
		}
		else if (input[i])
		{
			i += check_q_special(input, i);
			ft_find_the_type(input, start, mini, index_num);
			start = i;
		}
	}
	return (mini);
}

t_minishell	*ft_find_the_type(char *input, int start,
	t_minishell *mini, int *index_num)
{
	if (input[start] == '|')
		mini = ft_get_pipe(input, start, mini, index_num);
	else if (input[start] == '<')
		mini = ft_get_redi_herodoc(input, start, mini, index_num);
	else if (input[start] == '>')
		mini = ft_get_redi_append(input, start, mini, index_num);
	else if (input[start] == '<')
		mini = ft_get_redi_in(input, start, mini, index_num);
	else if (input[start] == '>')
		mini = ft_get_redi_out(input, start, mini, index_num);
	return (mini);
}

t_minishell	*ft_add_new_node(int start, int len,
	t_minishell *mini, int *index_num)
{
	char	*input;
	char	*sub_str;
	t_list	*new;

	input = mini->str2;
	sub_str = ft_substr(input, start, len);
	new = ft_lstnew(sub_str);
	ft_lstadd_back(&mini->nodes_t, new);
	new->index = *index_num;
	new->content = malloc((ft_strlen(sub_str) + 1) * sizeof(char));
	ft_strlcpy(new->content, sub_str, ft_strlen(sub_str) + 1);
	free(sub_str);
	*index_num = *index_num + 1;
	return (mini);
}

t_minishell	*ft_get_redi_in(char *input, int start,
	t_minishell *mini, int *index_num)
{
	char	*sub_str;
	t_list	*new;

	sub_str = ft_substr(input, start, 1);
	new = ft_lstnew(sub_str);
	new->index = *index_num;
	ft_lstadd_back(&mini->nodes_t, new);
	new->content = malloc((ft_strlen(sub_str) + 1) * sizeof(char));
	ft_strlcpy(new->content, sub_str, ft_strlen(sub_str) + 1);
	free(sub_str);
	*index_num = *index_num + 1;
	return (mini);
}
