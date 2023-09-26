/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 15:46:38 by pedro             #+#    #+#             */
/*   Updated: 2023/07/15 13:41:04 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Checks for unclosed quotes */
bool	verify_quotes(t_minishell *minishell)
{
	int	i;
	int	j;

	j = 0;
	while (j < minishell->nbr_tokens)
	{
		i = 0;
		while (minishell->token[j]->element[i])
		{
			if (minishell->token[j]->element[i] == '\'')
				i = skip_quotes(minishell->token[j]->element, i + 1, '\'');
			else if (minishell->token[j]->element[i] == '\"')
				i = skip_quotes(minishell->token[j]->element, i + 1, '\"');
			if (i == -1)
			{
				error_msg("Syntax error: Unclosed Quotes", NULL, NULL, 2);
				return (false);
			}
			else
				i++;
		}
		j++;
	}
	return (true);
}

void	lexer(t_minishell *minishell)
{
	minishell->token = split_to_tokens(minishell->full_prompt);
	if (!minishell->token)
		return ;
	while (minishell->token[minishell->nbr_tokens])
		minishell->nbr_tokens++;
	set_token_type(minishell->token);
	if (!verify_quotes(minishell))
	{
		clean_tokens(minishell->token);
		minishell->token = NULL;
		return ;
	}
}
