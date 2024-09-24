/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: murathanelcuman <murathanelcuman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:35:38 by murathanelc       #+#    #+#             */
/*   Updated: 2024/09/24 20:39:30 by murathanelc      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_return_fd(t_minishell *mini)
{
	dup2(mini->fd->out, STDOUT_FILENO);
	close(mini->fd->out);
	dup2(mini->fd->in, STDIN_FILENO);
	close(mini->fd->in);
}

// free open pipes
void	ft_free_open_pipes(int **fd_pipe)
{
	int	i;

	i = 0;
	while (fd_pipe[i])
	{
		free(fd_pipe[i]);
		i++;
	}
	free(fd_pipe);
}

int	**ft_open_pipe(t_minishell *mini)
{
	int	**pipe_fd;
	int	i;
	int	j;

	i = mini->token_num - 1; // öylesine
	pipe_fd = malloc((i + 1) * sizeof(int *));
	if (pipe_fd != NULL)
		ft_memset(pipe_fd, 0, (i + 1) * sizeof(int *));
	j = 0;
	while (j < i)
	{
		pipe_fd[j] = malloc(sizeof(int) * 2);
		pipe(pipe_fd[j]);
		j++;
	}
	return (pipe_fd);
}

void	ft_write_pipe(t_minishell *mini, t_parse *parse, int **fd_pipe, int i, int flag)
{
	if (parse->next == NULL)
	{
		dup2(mini->fd->out, STDOUT_FILENO);
		parse->out_file = mini->nodes_p->out_file;
		ft_execute_commands(mini, flag);
		close(mini->fd->out);
		close(fd_pipe[i - 1][0]);
		return ;
	}
	else
	{
		dup2(fd_pipe[i][1], STDOUT_FILENO);
		parse[i].out_file = fd_pipe[i][1];
		ft_execute_commands(mini, flag);
		close(fd_pipe[i][1]);
	}
	if (i > 0)
		close(fd_pipe[i - 1][0]);
}

// connect pipes
void	ft_connect_pipes(t_minishell *mini, t_parse *parse,int **fd_pipe, int i)
{
	if (mini->nodes_p == NULL)
	{
		dup2(mini->fd->in, STDIN_FILENO);
		close(mini->fd->in);
		return ;
	}
	if (i > 0)
	{
		dup2(fd_pipe[i - 1][0], STDIN_FILENO);
		parse[i].in_file = fd_pipe[i - 1][0];
	}
}

// handle pipe
void	ft_handle_pipe(t_minishell *mini, int flag)
{
	int	i;
	int	**fd_pipe;

	i = 0;
	fd_pipe = ft_open_pipe(mini);
	while (mini->nodes_p->args[i])
	{
		ft_write_pipe(mini, mini->nodes_p, fd_pipe, i, flag);
		i++;
		ft_connect_pipes(mini, mini->nodes_p, fd_pipe, i);
	}
	ft_free_open_pipes(fd_pipe);
}
