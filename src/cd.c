/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 12:42:49 by psotto-m          #+#    #+#             */
/*   Updated: 2023/07/15 15:44:43 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Updates PWD and OLDPWD when changing dirs */
void	ft_cd_update_pwds(t_list *envp)
{
	t_list	*tmp;
	t_list	*tmp2;

	tmp = envp;
	while (tmp)
	{
		if (!ft_strncmp(((t_env *)tmp->content)->name, "OLDPWD", 7))
		{
			free(((t_env *)tmp->content)->value);
			((t_env *)tmp->content)->value = find_in_env("PWD", envp);
			((t_env *)tmp->content)->exported = true;
			tmp2 = envp;
			while (tmp2)
			{
				if (!ft_strncmp(((t_env *)tmp2->content)->name, "PWD", 4))
				{
					((t_env *)tmp2->content)->value = getcwd(0, 0);
					((t_env *)tmp2->content)->exported = true;
					return ;
				}
				tmp2 = tmp2->next;
			}
		}
		tmp = tmp->next;
	}
}

void	ft_cd_no_args(t_list *envp)
{
	t_list	*tmp;

	tmp = envp;
	while (tmp)
	{
		if (!ft_strncmp(((t_env *)tmp->content)->name, "HOME", 5) && \
		((t_env *)tmp->content)->exported)
		{
			chdir(((t_env *)tmp->content)->value);
			return ;
		}
		tmp = tmp->next;
	}
	error_msg("cd: HOME not set", NULL, NULL, 1);
}

/* When cd args is either '-' or '~' it expands it */
char	*expand_path(char *arg, t_minishell *minishell)
{
	char	*path;
	char	*tmp;

	path = NULL;
	if (!ft_strncmp(arg, "-", 2))
	{
		path = ft_strdup(find_in_env("OLDPWD", minishell->envp));
		if (path)
			printf("%s\n", path);
		if (!path)
			error_msg("cd: OLDPWD not set", NULL, NULL, 1);
	}
	else if (arg[0] == '~')
	{
		tmp = ft_strdup(minishell->home);
		path = ft_strjoin(tmp, arg + 1);
		free(tmp);
	}
	return (path);
}

void	ft_cd(t_commands *cmd, t_list *envp, t_minishell *minishell)
{
	char	*path;

	if (cmd->nbr_args > 1)
	{
		error_msg("cd: ", "too many arguments", NULL, 1);
		return ;
	}
	if (!cmd->nbr_args)
		ft_cd_no_args(envp);
	else if (cmd->args[1])
	{
		if (!ft_strncmp(cmd->args[1], "-", 2) || cmd->args[1][0] == '~')
			path = expand_path(cmd->args[1], minishell);
		else
			path = ft_strdup(cmd->args[1]);
		if (path && chdir(path) < 0)
		{
			error_msg("cd: ", cmd->args[1], ": No such file or directory", 1);
			free(path);
			return ;
		}
		free(path);
	}
	ft_cd_update_pwds(envp);
}
