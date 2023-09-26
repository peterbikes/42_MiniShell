/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 11:33:06 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/05 11:36:06 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del(void *content)
{
	free(content);
}

void	*clean_tokens(t_element **token)
{
	int	i;

	if (!token)
		return (NULL);
	i = -1;
	while (token[++i])
		token[i]->element = ft_free(token[i]->element);
	token = ft_db_free(token);
	return (token);
}

void	*clean_cmds(t_commands **cmds)
{
	int	i;

	if (!cmds)
		return (NULL);
	i = -1;
	while (cmds[++i])
	{
		ft_lstclear(&cmds[i]->vars, del);
		ft_lstclear(&cmds[i]->redirects, del);
		cmds[i]->args = ft_db_free(cmds[i]->args);
		cmds[i]->cmd_path = ft_free(cmds[i]->cmd_path);
		cmds[i] = ft_free(cmds[i]);
	}
	cmds = ft_free(cmds);
	return (cmds);
}

void	clean_env(t_list **envp)
{
	t_list	*tmp;

	tmp = *envp;
	while (tmp)
	{
		free(((t_env *)tmp->content)->name);
		((t_env *)tmp->content)->name = NULL;
		free(((t_env *)tmp->content)->value);
		((t_env *)tmp->content)->value = NULL;
		tmp = tmp->next;
	}
	ft_lstclear(envp, del);
	envp = NULL;
}

void	*clean_all(t_minishell *minishell, int exit)
{
	if (minishell->full_prompt)
		minishell->full_prompt = ft_free(minishell->full_prompt);
	if (minishell->token)
		minishell->token = clean_tokens(minishell->token);
	if (minishell->commands)
		minishell->commands = clean_cmds(minishell->commands);
	if (exit)
	{
		if (minishell->envp)
			clean_env(&minishell->envp);
		minishell->home = ft_free(minishell->home);
	}
	return (NULL);
}
