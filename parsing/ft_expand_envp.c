/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_envp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasari <sebasari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 00:07:24 by murathanelc       #+#    #+#             */
/*   Updated: 2024/09/29 09:19:13 by sebasari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_envp_var(char *envp, int i)
{
	char	*str;
	int		j;
	int		k;

	i++;
	j = i;
	k = 0;
	while (envp[j])
	{
		k++;
		j++;
	}
	str = malloc(sizeof(char) * k + 1);
	if (!str)
		return (NULL);
	k = 0;
	while (envp[i])
	{
		str[k] = envp[i];
		k++;
		i++;
	}
	str[k] = 0;
	return (str);
}

// check if '?$' is entered
char	*ft_check_envp_exit_stat(char *str, int *i)
{
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(g_minishell.exit_status));
	}
	else if (str[*i] == '\0')
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	else
	{
		if (str[*i] == '"' || str[*i] == '\'' || str[*i] == ' ')
			return (ft_strdup("$"));
		else
		{
			while (ft_isalnum(str[*i]))
				(*i)++;
			return (ft_strdup(""));
		}
	}
}

// Handle environment variable
char	*ft_handle_envp_var(char **envp, char *str, int *i)
{
	char	*temp;
	int		j;
	int		k;
	int		p;

	j = 0;
	while (envp[j])
	{
		k = *i;
		p = 0;
		while (str[k] == envp[j][p])
		{
			if (ft_isalnum(str[k + 1]) == 0 && envp[j][k + 1] == '=')
			{
				*i = k + 1;
				return (ft_envp_var(envp[j + 1], p + 1));
			}
			p++;
			k++;
		}
	}
	temp = ft_check_envp_exit_stat(str, i);
	return (temp);
}

char	*ft_check_string(char *str, int *i)
{
	char	**envp;
	char	*new_str;

	envp = g_minishell.envp;
	if (str[*i] == '$')
	{
		(*i)++;
		return (ft_handle_envp_var(envp, str, i));
	}
	else if (*i == 0 && ft_is_quotes_there_index(str[*i]))
	{
		new_str =ft_substr(str, 0, ft_strlen(str));
		(*i) += ft_strlen(str);
		return (new_str);
	}
	else
	{
		new_str = ft_convert_char_to_string(str[*i]);
		(*i)++;
	}
	return (new_str);
}

char	**ft_search_envp_vars(char **str)
{
	char	*temp;
	char	*new_str;
	char	*check_str;
	int		i;
	int		j;

	i = 0;
	while (str[i])
	{
		j = 0;
		temp = ft_calloc(1, sizeof(char));
		if (!temp)
			return (NULL);
		while (str[i][j])
		{
			check_str = ft_check_string(str[i], &j);
			new_str = ft_strjoin(temp, new_str);
			temp = ft_strdup(new_str);
			free(check_str);
			free(new_str);
		}
		ft_modify_string(&str[i], &temp);
		i++;
		if (ft_strncmp(str[i - 1], "<<", ft_strlen("<<")) == 0 && str[i] != NULL)
			i++;
		// g_minishell.input = str;
	}
	return (str);
}
