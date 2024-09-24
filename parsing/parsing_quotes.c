/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: murathanelcuman <murathanelcuman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 13:44:46 by sebasari          #+#    #+#             */
/*   Updated: 2024/09/19 19:02:54 by murathanelc      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_getridof_q(t_list *nodes_t)
{
	int		i;
	int		j;
	char	*str;
	char	*new_str;
	int		len;

	len = 0;
	i = 0;
	j = 0;
	str = (char *)nodes_t->content;
	while (str[i])
	{
		if (ft_is_quotes_there_index(str[i]))
			len++;
		i++;
	}
	new_str = malloc(sizeof(char) * (ft_strlen(str) - len + 1));
	i = 0;
	while (str[i])
	{
		if (ft_is_quotes_there_index(str[i]))
			i++;
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	free(str);
	nodes_t->content = malloc(sizeof(char) * (ft_strlen(str) - len + 1));
	ft_strlcpy(nodes_t->content, new_str, ft_strlen(new_str) + 1);
	free(new_str);
	return (nodes_t);
}

int	ft_find_next_q(int start, char *input)
{
	int	next_q;

	next_q = start;
	if (input[start] == '\'')
	{
		next_q++;
		if (ft_single_quotes_finised(input, start))
			ft_error();
		while (input[next_q] != '\'' && input[next_q] != '\n' && input[next_q] != '\0')
			next_q++;
	}
	else if (input[start] == '\"')
	{
		next_q++;
		if (ft_double_quotes_finised(input, start))
			ft_error();
		while (input[next_q] != '\"' && input[next_q] != '\n' && input[next_q] != '\0')
			next_q++;
	}
	return (next_q);
}

t_list	*ft_basic_q(t_list *nodes_t, int len)
{
	char	*new_str;
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = (char *)nodes_t->content;
	new_str = malloc(sizeof(char) * (len - 1));
	while (str[i])
	{
		if (ft_is_quotes_there_index(str[i]))
			i++;
		else 
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	free(str);
	nodes_t->content = malloc(sizeof(char) * (len - 1));
	ft_strlcpy(nodes_t->content, new_str, ft_strlen(new_str) + 1);
	free(new_str);
	return (nodes_t);
}

t_list	*add_q_to_nodes(int *index, char *input, t_list *mini_list)
{
	int		next_q;
	t_list	*tmp;

	tmp = mini_list;
	next_q = ft_find_next_q(*index, input);
	if (*index == 0 && ((int)ft_strlen(input) - 1) == next_q)
	{
		tmp = ft_basic_q(tmp, next_q);
		*index = next_q;
		return (tmp);
	}
	tmp = ft_getridof_q(tmp);
	*index = next_q;
	return (tmp);
}

int	ft_is_quotes_there_index(char c)
{
	
	if (c == '\0') {
		printf("%c \n", c);
		return (0);
	}
	if (c == '\''){
		printf("%c \n", c);
		return (1);
	}
	else if (c == '\"')
		return (1);
	return (0);
}
