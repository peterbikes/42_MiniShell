/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 12:10:51 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/03 14:02:58 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	echo_nl_check(char *str)
{
	int	j;

	j = 1;
	while (str[j] == 'n')
		j++;
	if (str[j] == '\0' && str[j - 1] == 'n')
		return (false);
	return (true);
}

void	ft_echo(t_commands *cmd, bool n_check, bool newline, int i)
{
	if (!cmd->args[0])
	{
		printf("\n");
		return ;
	}
	while (cmd->args[i + 1] && cmd->args[i + 1][0] == '-')
	{
		n_check = echo_nl_check(cmd->args[i + 1]);
		if (!n_check)
			newline = false;
		if (!n_check)
			i++;
		else
			break ;
	}
	while (cmd->args[0] && cmd->args[++i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
	}
	if (newline == true)
		printf("\n");
}
