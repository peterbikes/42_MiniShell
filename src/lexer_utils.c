/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 14:37:56 by psotto-m          #+#    #+#             */
/*   Updated: 2023/07/03 13:31:48 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_quotes(const char *str, int i, char c)
{
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == '\0')
		return (-1);
	return (i);
}

int	redirect_size(const char *s, int i)
{
	if ((s[i] == REDIR_IN && s[i + 1] == REDIR_IN) || \
	(s[i] == REDIR_OUT && s[i + 1] == REDIR_OUT))
		return (2);
	else
		return (1);
}

void	redirect_tokens(const char *s, t_lexer_helper *lh)
{
	if ((s[lh->counter] == REDIR_IN && s[lh->counter + 1] == REDIR_IN) || \
	(s[lh->counter] == REDIR_OUT && s[lh->counter + 1] == REDIR_OUT))
	{
		lh->words++;
		lh->counter++;
	}
	else
		lh->words++;
}

static void	set_vars_types(t_element **token)
{
	char	**var;
	int		i;

	i = -1;
	while (token[++i])
	{
		if (token[i]->type == HEREDOC || token[i]->type == REDIR_IN || \
		token[i]->type == REDIR_OUT || token[i]->type == DBLREDIR_OUT)
			i++;
		else if (token[i]->type == WORD)
		{
			var = ft_split(token[i]->element, '=');
			if (token[i]->element[0] != '=' && \
			ft_strchr(token[i]->element, '=') && \
			export_name_validation(var[0], 0))
				token[i]->type = VAR;
			else if (!token[i + 1] || token[i + 1]->type == WORD)
			{
				ft_db_free(var);
				break ;
			}
			ft_db_free(var);
		}
	}
}

void	set_token_type(t_element **tokens)
{
	int		i;

	if (!tokens)
		return ;
	i = -1;
	while (tokens[++i])
	{
		if (tokens[i]->element[0] == PIPE)
			tokens[i]->type = PIPE;
		else if (tokens[i]->element[0] == REDIR_IN && !tokens[i]->element[1])
			tokens[i]->type = REDIR_IN;
		else if (tokens[i]->element[0] == REDIR_OUT && !tokens[i]->element[1])
			tokens[i]->type = REDIR_OUT;
		else if (tokens[i]->element[0] == REDIR_IN && tokens[i]->element[1])
			tokens[i]->type = HEREDOC;
		else if (tokens[i]->element[0] == REDIR_OUT && tokens[i]->element[1])
			tokens[i]->type = DBLREDIR_OUT;
		else if (ft_strchr(tokens[i]->element, '$'))
			tokens[i]->env = true;
	}
	set_vars_types(tokens);
}
