/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 13:03:03 by psotto-m          #+#    #+#             */
/*   Updated: 2023/07/15 11:16:54 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_c(int signal)
{
	if (signal != SIGINT)
		return ;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit.status = signal + 128;
}

void	hdoc_handle_c(int signal)
{
	if (signal != SIGINT)
		return ;
	printf("\n");
	g_exit.status = signal + 128;
	ft_exit(g_exit.minishell, NULL, 0);
}

void	handle_child(int signal)
{
	if (signal == SIGINT)
		printf("\n");
	else if (signal == SIGQUIT)
		printf("Quit (core dumped)\n");
	g_exit.status = signal + 128;
}

/* Sets the signal handling for the main process */
void	init_signals(void)
{
	signal(SIGINT, handle_c);
	signal(SIGQUIT, SIG_IGN);
}
