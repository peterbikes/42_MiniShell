/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 21:39:51 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/15 15:19:38 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Converts the env list to an array */
char	**env_to_array(t_list *envp)
{
	char	**env;
	char	*str;
	t_list	*tmp;
	int		size;
	int		i;

	size = ft_lstsize(envp);
	env = ft_calloc(size + 1, sizeof(*env));
	i = 0;
	tmp = envp;
	while (tmp)
	{
		if (((t_env *)tmp->content)->exported)
		{
			str = ft_strjoin(((t_env *)tmp->content)->name, "=");
			env[i] = ft_strjoin(str, ((t_env *)tmp->content)->value);
			free(str);
			i++;
		}
		tmp = tmp->next;
	}
	return (env);
}

/* Prints the env list */
void	ft_env(t_commands *cmds, t_list *envp)
{
	t_list	*tmp;

	if (cmds->nbr_args > 0)
	{
		if (access(cmds->args[1], F_OK == 0) && \
		(ft_strrchr(cmds->args[1], '/') || ft_strrchr(cmds->args[1], '.')))
			error_msg("env: '", cmds->args[1], "': Permission denied", 126);
		else
			error_msg("env: '", cmds->args[1], \
			"': No such file or directory", 127);
		return ;
	}
	tmp = envp;
	while (tmp)
	{
		if (((t_env *)tmp->content)->value && \
		((t_env *)tmp->content)->exported)
			printf("%s=%s\n", ((t_env *)tmp->content)->name, \
			((t_env *)tmp->content)->value);
		tmp = tmp->next;
	}
}

/* Copies the environment variables to a linked list */
t_list	*duplicate_envp(char **envp, char **home)
{
	t_env	*content;
	t_list	*env;
	char	**var;
	int		i;

	env = NULL;
	i = -1;
	while (envp[++i])
	{
		var = ft_split(envp[i], '=');
		content = ft_calloc(1, sizeof(*content));
		content->name = ft_strdup(var[0]);
		content->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		content->exported = true;
		content->system = true;
		if (!ft_strncmp(var[0], "HOME", 5))
			*home = ft_strdup(content->value);
		ft_db_free(var);
		ft_lstadd_back(&env, ft_lstnew(content));
	}
	return (env);
}

char	*find_in_env(char *var, t_list *env)
{
	t_list	*curr;
	char	*str;

	curr = env;
	str = NULL;
	while (curr)
	{
		if (((t_env *)curr->content)->exported)
			if (!ft_strncmp(var, ((t_env *)curr->content)->name, \
			ft_strlen(var) + 1))
				str = ((t_env *)curr->content)->value;
		curr = curr->next;
	}
	return (str);
}
