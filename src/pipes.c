/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 12:48:10 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/15 15:50:32 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Creates an array of pipes[2] */
int	**open_pipes(int **pipes, int nbr_pipes)
{
	int	i;

	pipes = ft_calloc(nbr_pipes, sizeof(int *));
	i = -1;
	while (++i < nbr_pipes)
	{
		pipes[i] = ft_calloc(2, sizeof(int));
		pipe(pipes[i]);
	}
	return (pipes);
}

/* Closes all pipes after idx command */
void	close_pipes(int i, int *pipe[2], int nbr_cmds)
{
	while (++i < nbr_cmds)
	{
		if (i == 0)
			close(pipe[i][READ]);
		else if (i == nbr_cmds - 1)
			break ;
		else
			close(pipe[i][READ]);
	}
}

void	free_pipes(t_minishell *minishell)
{
	int	i;

	if (!minishell->pipe)
		return ;
	i = -1;
	while (++i < minishell->nbr_pipes)
		minishell->pipe[i] = ft_free(minishell->pipe[i]);
	minishell->pipe = ft_free(minishell->pipe);
}

/* Sets the cmd fd in and out to the pipeline and closes all the pipes after */
void	set_pipes(t_commands *cmd, int *pipe[2], int idx, int nbr_cmds)
{
	if (idx == 0)
	{
		cmd->fd_out = dup(pipe[idx][WRITE]);
		close(pipe[idx][WRITE]);
	}
	else if (idx == nbr_cmds - 1)
	{
		cmd->fd_in = dup(pipe[idx - 1][READ]);
		close(pipe[idx - 1][READ]);
	}
	else
	{
		cmd->fd_out = dup(pipe[idx][WRITE]);
		close(pipe[idx][WRITE]);
		cmd->fd_in = dup(pipe[idx - 1][READ]);
		close(pipe[idx - 1][READ]);
	}
}

void	dupping(t_commands **cmd, int idx)
{
	if (cmd[idx]->fd_in > STDIN_FILENO)
	{
		dup2(cmd[idx]->fd_in, STDIN_FILENO);
		close(cmd[idx]->fd_in);
	}
	if (cmd[idx]->fd_out > STDOUT_FILENO)
	{
		dup2(cmd[idx]->fd_out, STDOUT_FILENO);
		close(cmd[idx]->fd_out);
	}
}
