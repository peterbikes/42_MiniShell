/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 11:23:43 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/03 13:58:52 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_vars(char *var, t_list *env, size_t size, int *j)
{
	t_list	*curr;
	char	*str;
	char	*tmp;

	curr = env;
	str = NULL;
	while (curr)
	{
		if (((t_env *)curr->content)->exported)
		{
			if (!ft_strncmp(var, ((t_env *)curr->content)->name, size) && \
			ft_strlen(((t_env *)curr->content)->name) == size)
				str = ((t_env *)curr->content)->value;
		}
		curr = curr->next;
	}
	if (!str)
	{
		*j -= 1;
		str = ft_strdup(var + size);
		return (str);
	}
	*j += ft_strlen(str) - 1;
	tmp = ft_strjoin(str, var + size);
	return (tmp);
}

char	*expand_vars(char *token, t_list *envp, int *j)
{
	int		i;
	char	*str;
	char	*tmp;
	char	*tmp1;

	i = -1;
	if (token[0] == '?')
	{
		tmp1 = ft_itoa(g_exit.status);
		tmp = ft_strjoin(tmp1, token + i + 2);
		free(tmp1);
		return (tmp);
	}
	if (ft_isdigit(token[0]))
	{
		if (token[0] == '0')
			return (ft_strjoin(("minishell"), token + 1));
		else
			return (ft_strjoin("", token + 1));
	}
	while (token[++i])
		if (!ft_isalnum(token[i]) && token[i] != '_')
			break ;
	str = get_vars(token, envp, i, j);
	return (str);
}

char	*expand_token(char *token, int *j, t_list *env, int expand)
{
	char	*str;
	char	*tmp;
	char	*tmp1;

	str = ft_calloc(*j + 1, sizeof(*str));
	ft_memmove(str, token, *j);
	if (expand)
		tmp1 = expand_vars(token + *j + 1, env, j);
	else
		tmp1 = ft_strdup(token + *j + 1);
	tmp = ft_strjoin(str, tmp1);
	free(tmp1);
	free(str);
	free(token);
	return (tmp);
}

char	*expansor(char *element, int *j, t_list *env, int *quote)
{
	if (element[*j] == SQUOTE && quote[1] == -1 && quote[2] == 1)
	{
		quote[0] *= -1;
		element = expand_token(element, j, env, 0);
		*j -= 1;
	}
	else if (element[*j] == DQUOTE && quote[0] == -1 && quote[2] == 1)
	{
		quote[1] *= -1;
		element = expand_token(element, j, env, 0);
		*j -= 1;
	}
	else if (element[*j] == ENV && (ft_isalnum(element[*j + 1]) || \
	element[*j + 1] == '_' || element[*j + 1] == '?') && quote[0] == -1 && \
	quote[2])
		element = expand_token(element, j, env, 1);
	return (element);
}

void	expansor_main(t_element **token, t_list *env)
{
	int		i;
	int		j;
	int		quote[3];

	quote[0] = -1;
	quote[1] = -1;
	quote[2] = 1;
	if (!token)
		return ;
	i = -1;
	while (token[++i])
	{
		if (token[i]->type == HEREDOC)
		{
			i++;
			quote[2] = 0;
		}
		j = -1;
		while (token[i] && token[i]->element[++j])
		{
			token[i]->element = expansor(token[i]->element, &j, env, quote);
		}
		quote[2] = 1;
	}
}
