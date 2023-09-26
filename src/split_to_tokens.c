/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_to_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 17:41:09 by psotto-m          #+#    #+#             */
/*   Updated: 2023/07/11 22:19:43 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_time(const char *s, t_lexer_helper *lhp)
{
	bool	flag;
	char	quote;

	flag = false;
	while (s[lhp->counter])
	{
		if (s[lhp->counter] == SQUOTE || s[lhp->counter] == DQUOTE)
			quote = s[lhp->counter];
		if (s[lhp->counter] == quote)
		{
			flag = true;
			lhp->counter++;
			while (s[lhp->counter] && s[lhp->counter] != quote)
				lhp->counter++;
			if (s[lhp->counter] == quote)
				flag = false;
		}
		if (!flag && (!s[lhp->counter + 1] || \
		(s[lhp->counter + 1] == ' ' || s[lhp->counter + 1] == '\t')))
			break ;
		else if (s[lhp->counter])
			lhp->counter++;
	}
}

void	ft_token_count(char *s, t_lexer_helper *lh)
{
	bool	flag;

	flag = true;
	while (s[lh->counter])
	{
		if (s[lh->counter] == SQUOTE
			|| s[lh->counter] == DQUOTE)
			quote_time(s, lh);
		if (s[lh->counter] != ' ' && s[lh->counter] != '\t' \
			&& s[lh->counter] != PIPE && s[lh->counter] != REDIR_IN
			&& s[lh->counter] != REDIR_OUT && flag == true)
		{
			flag = false;
			lh->words++;
		}
		if (s[lh->counter] == PIPE || s[lh->counter] == REDIR_IN \
		|| s[lh->counter] == REDIR_OUT)
			redirect_tokens(s, lh);
		if (s[lh->counter] == ' ' || s[lh->counter] == '\t' \
		|| s[lh->counter] == PIPE || s[lh->counter] == REDIR_IN \
		|| s[lh->counter] == REDIR_OUT)
			flag = true;
		if (s[lh->counter])
			lh->counter++;
	}
}

int	ft_word_size(char *s, int i, t_lexer_helper	*lexer_helper)
{
	int	word_size;

	lexer_helper->counter = 0;
	word_size = 0;
	while (s[i])
	{
		if (s[i] == SQUOTE || s[i] == DQUOTE)
		{
			lexer_helper->counter = i;
			quote_time(s, lexer_helper);
			return (lexer_helper->counter - i + word_size + 1);
		}
		if ((s[i] != ' ' && s[i] != '\t') && s[i] != PIPE
			&& s[i] != REDIR_IN && s[i] != REDIR_OUT)
			word_size++;
		if (s[i] == PIPE || s[i] == REDIR_IN || s[i] == REDIR_OUT)
			return (redirect_size(s, i));
		i++;
		if (s[i] == ' ' || s[i] == '\t' || s[i] == PIPE \
		|| s[i] == REDIR_IN || s[i] == REDIR_OUT)
			return (word_size);
	}
	return (word_size);
}

void	tokens_fill(t_element **tokens, char *s, t_lexer_helper *lexer_ptr)
{
	int	i;
	int	k;

	k = 0;
	i = 0;
	while (k < lexer_ptr->words)
	{
		while (s[i] && (s[i] == '\t' || s[i] == ' '))
			i++;
		tokens[k] = ft_calloc(ft_word_size(s, i, lexer_ptr) + 1, \
		sizeof(*tokens));
		tokens[k]->element = ft_substr(s, i, ft_word_size(s, i, lexer_ptr));
		i += ft_strlen(tokens[k]->element);
		k++;
	}
}

t_element	**split_to_tokens(char *prompt)
{
	t_element		**element;
	t_lexer_helper	lexer_helper;

	if (!*prompt)
		return (NULL);
	lexer_helper.words = 0;
	lexer_helper.counter = 0;
	ft_token_count(prompt, &lexer_helper);
	element = ft_calloc((lexer_helper.words + 1), sizeof(**element));
	tokens_fill(element, prompt, &lexer_helper);
	return (element);
}
