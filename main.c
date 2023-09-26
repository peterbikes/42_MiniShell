/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 11:15:30 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/15 16:03:59 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exit	g_exit;

/* Fills minishell main struct and consequent sub-structs */
void	fill_minishell(t_minishell *minishell, char *prompt)
{
	if (!prompt)
		minishell->full_prompt = NULL;
	else
		minishell->full_prompt = ft_strdup(prompt);
	minishell->token = NULL;
	minishell->commands = NULL;
	minishell->pipe = NULL;
	minishell->nbr_tokens = 0;
	minishell->nbr_commands = 0;
	minishell->nbr_pipes = 0;
	minishell->cmd_idx = -1;
}

int	main(int ac, char **av, char **envp)
{
	char		*prompt;
	t_minishell	minishell;

	(void)ac;
	(void)av;
	minishell.fd_in = dup(STDIN_FILENO);
	minishell.fd_out = dup(STDOUT_FILENO);
	g_exit.minishell = &minishell;
	init_signals();
	minishell.envp = duplicate_envp(envp, &minishell.home);
	while (1)
	{
		prompt = readline(GREEN "Mini"YELLOW"Shell"CYAN"~>"RESET" ");
		fill_minishell(&minishell, prompt);
		if (!prompt)
			ft_exit(&minishell, NULL, 1);
		if (*prompt)
			add_history(prompt);
		free(prompt);
		lexer(&minishell);
		expansor_main(minishell.token, minishell.envp);
		parser(&minishell, minishell.token);
		executor_main(&minishell);
		clean_all(&minishell, false);
	}
}
