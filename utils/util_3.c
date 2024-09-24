/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: murathanelcuman <murathanelcuman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 17:25:43 by sebasari          #+#    #+#             */
/*   Updated: 2024/09/21 14:25:04 by murathanelc      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void printCharArray(char **array)
{
	int i = 0;
	while (array[i] != NULL)
	{
		printf("%s\n", array[i]);
		i++;
	}
}

int	ft_controll_qoutes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

int	ft_special_type_index(char c)
{

	if (c == '<' || c == '>' || c == '|')
			return (1);
	return (0);
}

int	ft_updated_strchr(char *str, int c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

void	ft_modify_string(char **old, char **new)
{
	free(*old);
	*old = ft_strdup(*new);
	free(*new);
}
