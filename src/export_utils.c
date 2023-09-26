/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 11:45:26 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/03 13:59:41 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Creates a copy of the env list */
t_list	*ft_copy_list(t_list **envp)
{
	t_list	*copy;
	t_list	*temp;
	t_env	*content;

	copy = NULL;
	temp = *envp;
	while (temp)
	{
		content = ft_calloc(1, sizeof(t_env));
		content->name = ft_strdup((((t_env *)temp->content)->name));
		content->exported = ((t_env *)temp->content)->exported;
		content->system = ((t_env *)temp->content)->system;
		if ((((t_env *)temp->content)->value))
			content->value = ft_strdup((((t_env *)temp->content)->value));
		else
			content->value = NULL;
		ft_lstadd_back(&copy, ft_lstnew(content));
		temp = temp->next;
	}
	return (copy);
}

/* Sorts the list by ASCII */
void	export_order(t_list **exp)
{
	t_list	*temp;
	t_env	aux;
	bool	sort;

	sort = true;
	temp = *exp;
	while (sort)
	{
		sort = false;
		while (temp && temp->next)
		{
			if (ft_strncmp(((t_env *)temp->content)->name, \
			((t_env *)temp->next->content)->name, \
			ft_strlen(((t_env *)temp->content)->name)) > 0)
			{
				aux = *((t_env *)temp->content);
				*((t_env *)temp->content) = *((t_env *)temp->next->content);
				*((t_env *)temp->next->content) = aux;
				sort = true;
			}
			temp = temp->next;
		}
		temp = *exp;
	}
}

/* Checks if a variable already exists and values has to be updated or if var is
 * and just needs to be added */
bool	export_check(t_list **envp, char **var, char *cmd, int export)
{
	t_list	*tmp;

	tmp = *envp;
	if (!export_name_validation(var[0], 1))
		return (false);
	while (tmp)
	{
		if (!ft_strncmp(((t_env *)tmp->content)->name, var[0], \
		ft_strlen(var[0]) + 1))
		{
			if (export)
				((t_env *)tmp->content)->exported = true;
			if (!var[1] && cmd[ft_strlen(cmd) - 1] == '=')
				var[1] = "\0";
			if (!var[1])
				return (false);
			ft_free(((t_env *)tmp->content)->value);
			((t_env *)tmp->content)->value = ft_strdup(ft_strchr(cmd, '=') + 1);
			return (false);
		}
		tmp = tmp->next;
	}
	return (true);
}
