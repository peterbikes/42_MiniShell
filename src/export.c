/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 21:39:51 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/15 13:41:04 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Prints the env list according to export standards */
void	ft_export_no_args(t_list **envp)
{
	t_list	*exp;
	t_list	*exp_head;

	exp = ft_copy_list(envp);
	export_order(&exp);
	exp_head = exp;
	while (exp)
	{
		if (((t_env *)exp->content)->exported)
		{
			printf("declare -x %s", ((t_env *)exp->content)->name);
			if (((t_env *)exp->content)->value)
				printf("=\"%s\"", ((t_env *)exp->content)->value);
			printf("\n");
		}
		free(((t_env *)exp->content)->name);
		if (((t_env *)exp->content)->value)
			free(((t_env *)exp->content)->value);
		exp = exp->next;
	}
	ft_lstclear(&exp_head, del);
}

/* Checks if the variable name is valid (Starts With '_' or alpha and only
 * contains alphanum chars and '_') */
bool	export_name_validation(char *key, int print)
{
	int	i;

	i = -1;
	while (key[++i])
	{
		if ((ft_isalnum(key[i]) == 0 && key[i] != '_') || \
		(ft_isalpha(key[0]) == 0 && key[0] != '_'))
		{
			if (print)
				error_msg("export: `", key, "': not a valid identifier", 1);
			return (false);
		}
	}
	return (true);
}

/* Adds a variable to the list */
void	add_export(t_list **envp, char *cmd, char *name, int export)
{
	t_env	*content;

	content = ft_calloc(1, sizeof(t_env));
	content->name = ft_strdup(name);
	if (export)
		content->exported = true;
	if (ft_strchr(cmd, '='))
		content->value = ft_strdup(ft_strchr(cmd, '=') + 1);
	ft_lstadd_back(envp, ft_lstnew(content));
}

void	ft_export(t_commands *cmd, t_list **envp, int export)
{
	char	**tmp;
	int		i;

	i = export - 1;
	if ((export && !cmd->nbr_args) || !*cmd->args[1])
	{
		ft_export_no_args(envp);
		return ;
	}
	while (cmd->args[++i])
	{
		if (cmd->args[i][0] == '=')
		{
			error_msg("export: `", cmd->args[i], \
			"': not a valid identifier", 1);
			break ;
		}
		tmp = ft_split(cmd->args[i], '=');
		if (export_check(envp, tmp, cmd->args[i], export))
			add_export(envp, cmd->args[i], tmp[0], export);
		ft_db_free(tmp);
	}
}
