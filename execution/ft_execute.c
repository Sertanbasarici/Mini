/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: murathanelcuman <murathanelcuman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 23:20:49 by murathanelc       #+#    #+#             */
/*   Updated: 2024/09/24 20:30:45 by murathanelc      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// return entered values as array list
char	**ft_get_char(t_minishell *token)
{
	char	**argv;
	t_list	*temp;
	int		argc;
	int		i;
	
	temp = token->nodes_t;
	argc = 0;
	i = 0;
	while (temp)
	{
		argc++;
		temp = temp->next;
	}
	temp = token->nodes_t;
	argv = malloc((argc + 1) *  sizeof(char *));
	if (!argv)
		return (NULL);
	while (i < argc && temp != NULL)
	{
		argv[i] = (char *)temp->content;
		temp = temp->next;
		i++;
	}
	argv[argc] = NULL;
	return (argv);
}

char	*ft_find_command_path(char *command)
{
    char		*path;
	char		*paths;
	char		*dir;
	static char	full_path[1024];
    // getenv -> set, unset, and fetch environment variables from the host environment list
    // return value of getenv: The getenv() function returns the value of the environment variable as a NUL-terminated
    // string.  If the variable name is not in the current environment, NULL is returned.
    path = getenv("PATH");
	if (!path)
		return (NULL);
	paths = ft_strdup(path);
	dir = strtok(paths, ":"); // ft_strtok hatalı
	while (dir != NULL)
	{
		snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
		if (access(full_path, X_OK) == 0)
		{
			free(paths);
			return (full_path);
		}
		dir = strtok(NULL, ":");
	}
	free(paths);
	return (NULL);
}

void	ft_execute_execve(t_minishell *mini)
{
	char	**argv;
	char	*command_path;
	char	*str;
	int		argc;
	int		i;
	t_list	*temp;
	
	temp = mini->nodes_t;
	str = (char *)temp->content;
	if (mini == NULL || str == NULL)
		return ;
	command_path = ft_find_command_path(str);
	if (command_path == NULL)
	{
		printf("%s: command not found\n", str);
		return ;
	}
	argc = 0;
	while (temp)
	{
		argc++;
		temp = temp->next;
	}
	argv = malloc((argc + 1) *  sizeof(char *));
	i = 0;
	temp = mini->nodes_t;
	while (i < argc && temp != NULL)
	{
		argv[i] = (char *)temp->content;
		temp = temp->next;
		i++;
	}
	argv[argc] = NULL; // last char shoudl be null
	if (execve(command_path, argv, NULL) == -1)
	{
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	free(argv);
}

// void	ft_builtin_commands(t_minishell *mini)
// {
// 	char	**input;

// 	input = ft_get_char(mini);
// 	if (ft_strncmp(input[0], "pwd", ft_strlen(str)) == 0)
// 		ft_pwd(*input);
// 	else if (ft_strncmp(input[0], "echo", ft_strlen(str)) == 0)
// 		ft_echo(input);
// 	else if (ft_strncmp(input[0], "cd", ft_strlen("cd")) == 0)
// 		ft_cd(input);
// 	else if (ft_strncmp(input[0], "env", ft_strlen("env")) == 0)
// 		ft_env(input);
// 	else if (ft_strncmp(input[0], "export", ft_strlen("export")) == 0)
// 		ft_export(input);
// 	else if (ft_strncmp(input[0], "unset", ft_strlen("unset")) == 0)
// 		ft_unset(input);
// 	else if (ft_strncmp(input[0], "exit", ft_strlen(str)) == 0)
// 		ft_exit(input);
// }

void	ft_execute_commands(t_minishell *mini, int flag)
{
	if (mini->file == NULL)
	{
		ft_execve_or_builtin(mini);
		if (flag == 0)
			ft_return_fd(mini);
		else
			dup2(mini->nodes_p->in_file, STDIN_FILENO);
		return ;
	}
	while (mini->file != NULL)
	{
		if (mini->file->type == GREATER)
			; // redirect in
		else if (mini->file->type == SMALLER)
			; // redirect out
		else if (mini->file->type == APPEND)
			; // append
		else if (mini->file->type == HERE_DOC)
			ft_heredoc(mini, mini->nodes_p, &mini->file, &mini->fd);
		if (mini->file == NULL && mini->nodes_p->next == NULL)
			ft_return_fd(mini);
	}
}

void	ft_execve_or_builtin(t_minishell *mini)
{
	pid_t	pid;
	int		type;
	char	**str;

	type = ft_builtin_or_not(mini->nodes_p->args[0]);
	str = mini->nodes_p->args;
	if (mini->token_num == 1 && type != 0)
	{
		ft_execute_builtins(str);
		return ;
	}
	// signal();
	pid = fork();
	if (pid == 0)
	{
		if (type != 0)
		{
			ft_execute_builtins(str);
			exit(0);
		}
		else
			ft_execute_execve(mini);
	}
	return ;
}

void	ft_command(t_minishell *mini)
{
	// int	type; // sinyal kısmıyla alakalı
	int	flag;

	// type = ft_builtin_or_not(mini->nodes_p->args[0]); bu kısımda sinyallerle ilgili //
	flag = 0;
	if (mini->nodes_p->next != NULL)
	{
		flag = 1;
		ft_handle_pipe(mini, flag);
	}
	else
		ft_execute_commands(mini, flag);
	// handle signals later //
}

void	ft_dup_fd(t_minishell *mini, t_parse *parse)
{
	if (parse->args[0])
	{
		parse->in_file = dup(mini->fd->in);
		parse->out_file = dup(mini->fd->out);
	}
	mini->fd->in = dup(STDIN_FILENO);
	mini->fd->out = dup(STDOUT_FILENO);
}

void	ft_execution(t_minishell *mini)
{
	t_parse	*parse;
	char	*str;

	parse = mini->nodes_p;
	str = (char *)mini->nodes_t->content;
	if (mini->nodes_t == NULL || str == NULL)
		return ;
	ft_dup_fd(mini, parse);
	// if (ft_strncmp(str, "ls", ft_strlen(str)) == 0)
	// 	ft_execute_commands(mini);
	// else
	// 	ft_builtin_commands(mini);
	ft_command(mini);
}
