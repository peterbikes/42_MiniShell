/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 12:47:24 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/15 15:51:15 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Gets path from env */
char	**get_path(t_list *envp)
{
	char	**path;
	char	*str;
	int		i;

	str = NULL;
	path = ft_split(find_in_env("PATH", envp), ':');
	if (!path)
		return (NULL);
	i = -1;
	while (path[++i])
	{
		str = ft_strjoin(path[i], "/");
		free(path[i]);
		path[i] = str;
	}
	return (path);
}

/* Gets absolute path for command */
char	*get_cmd_path(t_list *envp, char *cmd)
{
	char	**path;
	char	*str;
	int		i;

	str = NULL;
	if (ft_strrchr(cmd, '/') || cmd[0] == '.')
	{
		if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0 && \
		ft_strncmp(cmd, ".", 2) && ft_strncmp(cmd, "..", 3))
			str = ft_strdup(cmd);
		return (str);
	}
	path = get_path(envp);
	if (!path || !*cmd)
		return (str);
	i = -1;
	while (path[++i])
	{
		str = ft_strjoin(path[i], cmd);
		if (access(str, F_OK) == 0)
			break ;
		str = ft_free(str);
	}
	ft_db_free(path);
	return (str);
}

/* Checks for errors with cmd */
void	exec_errors(char *cmd_path, char *args)
{
	char	*str;

	str = NULL;
	if (cmd_path == NULL && ft_strchr(args, '/'))
	{
		if (access(args, F_OK) != 0)
			g_exit.status = 127;
		else if (access(args, X_OK) != 0)
			g_exit.status = 126;
		str = ft_strjoin("minishell: ", args);
		perror(str);
		free(str);
		return ;
	}
	if (cmd_path && access(cmd_path, X_OK) != 0)
	{
		g_exit.status = 126;
		str = ft_strjoin("minishell: ", cmd_path);
		perror(str);
		free(str);
		return ;
	}
}

/* Executes command with execve */
void	exec_cmd(t_minishell *minishell, t_commands *cmd)
{
	char		**env;
	struct stat	buf;

	cmd->cmd_path = get_cmd_path(minishell->envp, cmd->args[0]);
	exec_errors(cmd->cmd_path, cmd->args[0]);
	env = env_to_array(minishell->envp);
	if (cmd->cmd_path)
	{
		stat(cmd->cmd_path, &buf);
		if (S_ISDIR(buf.st_mode))
			error_msg(cmd->args[0], ": Is a directory", NULL, 126);
		else if (S_ISREG(buf.st_mode))
			execve(cmd->cmd_path, cmd->args, env);
	}
	else if (!cmd->cmd_path && !ft_strchr(cmd->args[0], '/'))
	{
		dup2(minishell->fd_in, STDIN_FILENO);
		dup2(minishell->fd_out, STDOUT_FILENO);
		g_exit.status = 127;
		printf("%s: command not found\n", \
		minishell->commands[minishell->cmd_idx]->args[0]);
	}
	ft_db_free(env);
}
