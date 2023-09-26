/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 12:53:55 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/15 15:53:49 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Checks syntax for redirects */
static bool	check_redirs(t_element *token)
{
	char	*tkn;

	if (token)
		tkn = token->element;
	else
		tkn = "newline";
	if (token && token->type == WORD)
		return (true);
	error_msg("syntax error near unexpected token `", \
		tkn, "'", 2);
	return (false);
}

/* Main syntax checker and prints error msg if syntax fails */
bool	syntax_check(t_element **token)
{
	int		i;

	i = -1;
	while (token[++i])
	{
		if (token[i]->type == PIPE)
		{
			if (token[i + 1] && token[i + 1]->type != PIPE && i)
				continue ;
			error_msg("syntax error near unexpected token `", \
			token[i]->element, "'", 2);
			return (false);
		}
		if (token[i]->type == HEREDOC || token[i]->type == REDIR_IN || \
		token[i]->type == REDIR_OUT || token[i]->type == DBLREDIR_OUT)
		{
			if (!check_redirs(token[i + 1]))
				return (false);
			else
				continue ;
		}
	}
	return (true);
}

/* Counts the nbr of arguments on the cmd, ignores env vars and redirects */
int	count_cmd_args(t_element **token)
{
	int	i;
	int	args;

	args = 0;
	i = -1;
	while (token[++i])
	{
		if (!token[i]->element[0])
			continue ;
		if (token[i]->type == PIPE)
			break ;
		if (token[i]->type == HEREDOC || token[i]->type == REDIR_IN || \
		token[i]->type == REDIR_OUT || token[i]->type == DBLREDIR_OUT)
			i++;
		else if (token[i]->type == WORD)
			args++;
	}
	return (args);
}

/* Count the nbr of pipes in prompt, used then to split the prompt in individual
 cmd */
void	count_forks(t_minishell *minishell, t_element **token)
{
	int	i;

	i = -1;
	if (!*token)
		return ;
	while (token[++i])
	{
		if (!token[i]->element[0])
			continue ;
		if (token[i]->type == PIPE)
			minishell->nbr_pipes++;
	}
	minishell->nbr_commands = minishell->nbr_pipes + 1;
}
