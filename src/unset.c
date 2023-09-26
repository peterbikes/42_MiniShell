/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 14:58:50 by psotto-m          #+#    #+#             */
/*   Updated: 2023/07/15 15:45:16 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_node(t_list **envp, t_list *prev, t_list *tmp)
{
	if (((t_env *)tmp->content)->system)
		((t_env *)tmp->content)->exported = false;
	else
	{
		if (tmp == prev)
			*envp = prev->next;
		else
			prev->next = prev->next->next;
		ft_free(((t_env *)tmp->content)->name);
		ft_free(((t_env *)tmp->content)->value);
		ft_lstdelone(tmp, del);
		tmp = NULL;
	}
}

/* Removes variables from the env list */
void	ft_unset(t_list **envp, t_commands *cmd)
{
	t_list	*tmp;
	t_list	*prev;
	int		i;

	if (!cmd->nbr_args)
		return ;
	i = 0;
	while (cmd->args[++i])
	{
		prev = *envp;
		tmp = *envp;
		if (!export_name_validation(cmd->args[i], 0))
			error_msg("unset: `", cmd->args[i], "': not a valid identifier", 1);
		while (tmp)
		{
			if (!ft_strncmp(((t_env *)tmp->content)->name, \
			cmd->args[i], ft_strlen(cmd->args[1]) + 1))
			{
				remove_node(envp, prev, tmp);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
	}
}
