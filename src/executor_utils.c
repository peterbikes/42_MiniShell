/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 14:21:22 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/03 15:55:20 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Simply checks if cmd is a built-in or not */
int	is_built_in(char *cmd)
{
	if (!cmd)
		return (1);
	if (!ft_strncmp(cmd, "echo", 5) || !ft_strncmp(cmd, "cd", 3) || \
	!ft_strncmp(cmd, "pwd", 4) || !ft_strncmp(cmd, "export", 7) || \
	!ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "env", 4) || \
	!ft_strncmp(cmd, "exit", 5))
		return (1);
	return (0);
}

static void	set_export(t_commands *cmd, t_list *envp, char **str, char *var)
{
	char	**tmp;
	int		i;

	if (!ft_strncmp(*cmd->args, "export", 7))
	{
		i = 0;
		while (cmd->args[++i])
		{
			tmp = ft_split(cmd->args[i], '=');
			if (!ft_strncmp(*str, *tmp, ft_strlen(*tmp) + 1))
			{
				if (export_check(&envp, str, var, 0))
					add_export(&envp, var, *str, 0);
				ft_db_free(tmp);
				break ;
			}
			ft_db_free(tmp);
		}
	}
}

/* Exports all variables to the env list and cleans the unwanted vars */
void	handle_vars(t_commands *cmd, t_list *envp)
{
	t_list	*tmp;
	char	**str;

	tmp = cmd->vars;
	if (*cmd->args == NULL)
	{
		while (tmp)
		{
			str = ft_split(tmp->content, '=');
			if (export_check(&envp, str, tmp->content, 0))
				add_export(&envp, tmp->content, str[0], 0);
			ft_db_free(str);
			tmp = tmp->next;
		}
	}
	else
	{
		while (tmp)
		{
			str = ft_split(tmp->content, '=');
			set_export(cmd, envp, str, tmp->content);
			ft_db_free(str);
			tmp = tmp->next;
		}
	}
}

/* Loops the redirects lists for every cmd struct and sets the fd's
 * accordingly */
void	handle_redirects(t_commands *cmd)
{
	t_list	*tmp;

	tmp = cmd->redirects;
	while (tmp)
	{
		if (!g_exit.status)
			redirects(cmd, ((t_redirect *)tmp->content)->type, \
			((t_redirect *)tmp->content)->file);
		tmp = tmp->next;
	}
}
