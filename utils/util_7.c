/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_7.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasari <sebasari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 19:17:03 by sebasari          #+#    #+#             */
/*   Updated: 2024/09/29 20:24:36 by sebasari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_q_begin_end(char *str, char *new_str, int len)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (str[i])
	{
		if (i == 0 || i == len - 1)
			i++;
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}

int	check_q_special(char *input, int i)
{
	if (input[i + 1] && ((input[i] == '<' && input[i + 1] == '<')
			|| (input[i] == '>' && input[i + 1] == '>')))
		return (2);
	else
		return (1);
}

int	ft_not_q(char *input, int i)
{
	while (input[i] && !ft_special_type_index(input[i]))
	{
		if (ft_is_quotes_there_index(input[i]))
		{
			i = ft_find_next_q(i, input);
			i++;
			return (i);
		}
		else
			return (1);
	}
	return (0);
}
