/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasari <sebasari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 12:27:07 by sebasari          #+#    #+#             */
/*   Updated: 2024/09/25 00:22:39 by sebasari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// global değişken
t_minishell	g_minishell;

int	check_if_empty(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

void	ctr_c(int num)
{
	(void)num;
	rl_on_new_line();
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ctrl_d(char *input)
{
	if (!input)
	{
		printf("exit\n");
		exit(130);
	}
}

int main(int argc, char **argv, char **envp)
{
	char	*input;

	(void)argc;
	(void)argv;
	ft_init(envp);
	signal(SIGINT, ctr_c);
	while (1) 
	{
		input = readline("minishell$ ");
		ctrl_d(input);
		if (check_if_empty(input) == 1)
			continue ;
		adjsut_all(input);
		add_history(input);
	}
	return 0;
}
