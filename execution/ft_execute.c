/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melcuman <melcuman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 23:20:49 by murathanelc       #+#    #+#             */
/*   Updated: 2024/09/23 14:50:03 by melcuman         ###   ########.fr       */
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

void	ft_execute_commands(t_minishell *mini)
{
	char	**argv;
	char	*command_path;
	char	*str;
	int		argc;
	pid_t	pid;
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
	// Create a child process
	pid = fork();
	// Check error
	if (pid < 0)
		perror("Fork failed");
	else if (pid == 0)
	{
		if (execve(command_path, argv, NULL) == -1)
		{
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
	}
	else
		wait(NULL); // Parent waits the child
	free(argv);
}

void	ft_builtin_commands(t_minishell *mini)
{
	char	*str;
	char	**input;

	input = ft_get_char(mini);
	if (ft_strncmp(input[0], "pwd", ft_strlen(str)) == 0)
		ft_pwd(*input);
	else if (ft_strncmp(input[0], "echo", ft_strlen(str)) == 0)
		ft_echo(input);
	else if (ft_strncmp(input[0], "cd", ft_strlen("cd")) == 0)
		ft_cd(input);
	else if (ft_strncmp(input[0], "env", ft_strlen("env")) == 0)
		ft_env(input);
	else if (ft_strncmp(input[0], "export", ft_strlen("export")) == 0)
		ft_export(input);
	else if (ft_strncmp(input[0], "unset", ft_strlen("unset")) == 0)
		ft_unset(input);
	else if (ft_strncmp(input[0], "exit", ft_strlen(str)) == 0)
		ft_exit(input);
}

void	ft_command(t_minishell *mini)
{
	t_list	*temp;
	char	*str;

	temp = mini->nodes_t;
	str = (char *)temp->content;
	if (temp == NULL || str == NULL)
		return ;
	if (ft_strncmp(str, "ls", ft_strlen(str)) == 0)
		ft_execute_commands(mini);
	else
		ft_builtin_commands(mini);
}
