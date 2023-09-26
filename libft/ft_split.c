/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 12:37:01 by heda-sil          #+#    #+#             */
/*   Updated: 2023/05/12 17:11:01 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_word_count(const char *s, char c)
{
	int	words;
	int	i;
	int	flag;

	flag = 0;
	words = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && !flag)
		{
			flag = 1;
			words++;
		}
		if (s[i] == c)
			flag = 0;
		i++;
	}
	return (words);
}

static int	ft_word_size(const char *s, char c)
{
	int	word_size;

	word_size = 0;
	while (*s)
	{
		if (*s != c)
			word_size++;
		s++;
		if (*s == c)
			return (word_size);
	}
	return (word_size);
}

static void	ft_free_sp(char **str, int i)
{
	while (i >= 0)
	{
		free(str[i]);
		i--;
	}
	free(str);
}

static char	**ft_print_arr(char **strs, int words, char const *s, char c)
{
	int	i;

	i = -1;
	while (++i < words)
	{
		while (*s == c)
			s++;
		strs[i] = ft_substr(s, 0, ft_word_size(s, c));
		while (*s != c && *s)
			s++;
		if (!strs[i])
		{
			ft_free_sp(strs, i);
			return (NULL);
		}
	}
	strs[i] = NULL;
	return (strs);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		words;

	if (!s)
		return (NULL);
	words = ft_word_count(s, c);
	strs = (char **)malloc((words + 1) * sizeof(*strs));
	if (!strs)
		return (NULL);
	ft_print_arr(strs, words, s, c);
	return (strs);
}
