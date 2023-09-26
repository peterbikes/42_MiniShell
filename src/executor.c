/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 00:01:48 by heda-sil          #+#    #+#             */
/*   Updated: 2023/09/26 16:51:52 by psotto-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Executes built-ins */
void	exec_built_ins(t_minishell *minishell, t_commands *cmd)
{
	char	*pwd;

	if (!ft_strncmp(cmd->args[0], "echo", 5))
		ft_echo(cmd, true, true, 0);
	if (!ft_strncmp(cmd->args[0], "cd", 3))
		ft_cd(cmd, minishell->envp, minishell);
	if (!ft_strncmp(cmd->args[0], "pwd", 4))
	{
		pwd = getcwd(0, 0);
		printf("%s\n", pwd);
		ft_free(pwd);
	}
	if (!ft_strncmp(cmd->args[0], "export", 7))
		ft_export(cmd, &minishell->envp, 1);
	if (!ft_strncmp(cmd->args[0], "unset", 6))
		ft_unset(&minishell->envp, cmd);
	if (!ft_strncmp(cmd->args[0], "env", 4))
		ft_env(cmd, minishell->envp);
	if (!ft_strncmp(cmd->args[0], "exit", 5))
		ft_exit(minishell, cmd, 1);
}

/* Forks */
pid_t	forking(t_minishell *minishell)
{
	pid_t	pid;

	signal(SIGINT, handle_child);
	signal(SIGQUIT, handle_child);
	pid = fork();
	if (pid < 0)
		perror("minishell: Fork");
	else if (!pid)
	{
		exec(minishell, minishell->cmd_idx);
		if (minishell->pipe)
			free_pipes(minishell);
		clean_all(minishell, true);
		exit(g_exit.status);
	}
	return (pid);
}

/* Determines which execution needs to be done, execve or
built-ins */
void	exec(t_minishell *minishell, int idx)
{
	handle_vars(minishell->commands[idx], minishell->envp);
	if (*minishell->commands[idx]->args)
	{
		if (minishell->commands[idx]->fd_in < 0 || \
		minishell->commands[idx]->fd_out < 0 || g_exit.status == 130)
			return ;
		if (minishell->nbr_pipes > 0)
			close_pipes(idx - 1, minishell->pipe, minishell->nbr_commands);
		if (is_built_in(minishell->commands[idx]->args[0]))
			exec_built_ins(minishell, minishell->commands[idx]);
		else
			exec_cmd(minishell, minishell->commands[idx]);
	}
	minishell->commands = clean_cmds(minishell->commands);
}

int	executor(t_minishell *minishell)
{
	pid_t	pid;

	pid = 0;
	g_exit.status = 0;
	if (minishell->nbr_pipes > 0)
		set_pipes(minishell->commands[minishell->cmd_idx], minishell->pipe, \
		minishell->cmd_idx, minishell->nbr_commands);
	handle_redirects(minishell->commands[minishell->cmd_idx]);
	dupping(minishell->commands, minishell->cmd_idx);
	if (minishell->nbr_pipes == 0 && \
	is_built_in(minishell->commands[minishell->cmd_idx]->args[0]))
		exec(minishell, minishell->cmd_idx);
	else
		pid = forking(minishell);
	dup2(minishell->fd_in, STDIN_FILENO);
	dup2(minishell->fd_out, STDOUT_FILENO);
	return (pid);
}

/* fork and execve here */
void	executor_main(t_minishell *minishell)
{
	int		status;
	pid_t	pid;

	if (minishell->commands == NULL)
		return ;
	pid = 0;
	status = 1;
	if (minishell->nbr_pipes > 0)
		minishell->pipe = open_pipes(minishell->pipe, minishell->nbr_pipes);
	while (++minishell->cmd_idx < minishell->nbr_commands)
		pid = executor(minishell);
	if (minishell->pipe)
		free_pipes(minishell);
	waitpid(pid, &status, 0);
	while (waitpid(0, NULL, 0) > 0)
		continue ;
	if (WIFEXITED(status))
		g_exit.status = WEXITSTATUS(status);
	init_signals();
}
