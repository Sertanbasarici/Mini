/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasari <sebasari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:47:24 by murathanelc       #+#    #+#             */
/*   Updated: 2024/09/15 14:35:34 by sebasari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	dir = ft_strtok(paths, ":");
	while (dir != NULL)
	{
		snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
		if (access(full_path, X_OK) == 0)
		{
			free(paths);
			return (full_path);
		}
		dir = ft_strtok(NULL, ":");
	}
	free(paths);
	return (NULL);
}

void	ft_execute_command(t_minishell *mini)
{
	char	**argv;
	char	*command_path;
	char	*str;
	int		argc;
	pid_t	pid;
	int		i;
	t_list	*temp;
	
	temp = mini->nodes_t;
	if (mini == NULL || (char *)temp->content == NULL)
		return ;
	str = (char *)temp->content;
	command_path = ft_find_command_path(str);
	if (command_path == NULL)
	{
		printf("%s: command not found", str);
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
