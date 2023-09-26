/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 12:01:25 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/15 15:53:56 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Initializes the cmd struct */
t_commands	*creat_cmd(t_commands *cmd, t_element **token)
{
	cmd = ft_calloc(count_cmd_args(token) + 1, sizeof(*cmd));
	cmd->nbr_args = count_cmd_args(token) - 1;
	cmd->args = ft_calloc(cmd->nbr_args + 2, sizeof(*cmd->args));
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	cmd->redirects = NULL;
	cmd->vars = NULL;
	return (cmd);
}

/* Cleans the commands form the redirs, AKA stores all redirs in a list to later
 * be treated */
void	clean_redirs(t_element **token, t_commands *cmd, int *k, int *j)
{
	if (token[*j] && token[*j]->type == WORD)
	{
		cmd->args[*k] = ft_strdup(token[*j]->element);
		(*k)++;
	}
	if (token[*j] && (token[*j]->type == HEREDOC || \
	token[*j]->type == REDIR_IN || token[*j]->type == REDIR_OUT || \
	token[*j]->type == DBLREDIR_OUT))
	{
		ft_lstadd_back(&cmd->redirects, \
		ft_lstnew(create_redirect(token[*j]->type, \
		token[*j + 1]->element)));
		(*j)++;
	}
}

/* Constructs the commands from the tokens, removes all redirections and env
 * variables and deals with them before moving to the execution */
t_commands	**extract_cmds(t_minishell *minishell, t_element **token)
{
	t_commands	**cmd;
	int			i;
	int			j;
	int			k;

	cmd = ft_calloc(minishell->nbr_commands + 1, sizeof(**cmd));
	i = -1;
	j = -1;
	while (++i < minishell->nbr_commands)
	{
		cmd[i] = creat_cmd(cmd[i], token + j + 1);
		k = 0;
		while (token[++j])
		{
			if (!token[j]->element[0])
				continue ;
			if (token[j]->type == VAR)
				ft_lstadd_back(&cmd[i]->vars, \
				ft_lstnew(ft_strdup(token[j]->element)));
			clean_redirs(token, cmd[i], &k, &j);
			if (token[j] && token[j]->type == PIPE)
				break ;
		}
	}
	return (cmd);
}

/* Checks syntax and constructs cmds */
void	parser(t_minishell *minishell, t_element **token)
{
	if (!token)
	{
		minishell->commands = NULL;
		return ;
	}
	if (!syntax_check(token))
	{
		minishell->commands = NULL;
		return ;
	}
	count_forks(minishell, token);
	minishell->commands = extract_cmds(minishell, token);
}
