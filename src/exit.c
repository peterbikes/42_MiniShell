/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 19:52:54 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/15 15:52:47 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_exit_arg_check(char *arg)
{
	int		i;
	char	*l_max;
	char	*l_min;

	l_max = "9223372036854775807";
	l_min = "-9223372036854775808";
	i = -1;
	if (arg[0] == '-' && ft_strlen(arg) >= 20 && \
	ft_strncmp(arg, l_min, ft_strlen(arg)) > 0)
		return (false);
	if (ft_strlen(arg) >= 19 && ft_strncmp(arg, l_max, ft_strlen(arg)) > 0)
		return (false);
	if (arg[0] == '+' || arg[0] == '-')
		i++;
	while (arg[++i])
	{
		if (ft_isdigit(arg[i]) == false)
			return (false);
	}
	return (true);
}

void	ft_exit(t_minishell *minishell, t_commands *cmd, int print)
{
	if (minishell->pipe)
		free_pipes(minishell);
	if (!minishell->nbr_pipes && print)
		printf("exit\n");
	if (cmd == NULL)
	{
		clean_all(minishell, 1);
		exit(g_exit.status % 256);
	}
	if (cmd->nbr_args > 1)
	{
		error_msg("exit: too many arguments", NULL, NULL, 1);
		return ;
	}
	if (cmd->args[1])
	{
		if (ft_exit_arg_check(cmd->args[1]) == false)
			error_msg("exit: ", cmd->args[1], \
			" : numeric argument required", 2);
		else
			g_exit.status = ft_atol(cmd->args[1]) % 256;
	}
	clean_all(minishell, 1);
	exit(g_exit.status % 256);
}

void	error_msg(char *msg, char *arg1, char *arg2, int error)
{
	g_exit.status = error;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd(arg1, STDERR_FILENO);
	ft_putstr_fd(arg2, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}
