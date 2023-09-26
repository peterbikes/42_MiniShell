/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 11:43:14 by heda-sil          #+#    #+#             */
/*   Updated: 2023/09/26 16:52:40 by psotto-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Creates and initializes a redirect struct */

char	*clean_limiter(char *limiter, int i, int j)
{
	char	*str;
	char	control;

	str = ft_calloc(limiter_size(limiter) + 1, sizeof(*str));
	control = '\0';
	while (limiter[i])
	{
		if (control != '\0' && limiter[i] == control)
		{
			control = '\0';
			i++;
			continue ;
		}
		if (control == '\0' && (limiter[i] == '\'' || limiter[i] == '\"'))
		{
			control = limiter[i];
			i++;
			continue ;
		}
		str[j] = limiter[i];
		j++;
		i++;
	}
	return (str);
}

char	*expand_hdoc(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && ((ft_isalnum(input[i + 1]) || \
	input[i + 1] == '_' || input[i + 1] == '?')))
			input = expand_token(input, &i, g_exit.minishell->envp, 1);
		i++;
	}
	return (input);
}

// Heredoc execution;
void	do_hdoc(int *hdoc, char *limiter, t_minishell *minishell)
{
	char	*line;
	char	*stop;

	stop = clean_limiter(limiter, 0, 0);
	while (1)
	{
		line = readline(">");
		if (!line || (!ft_strncmp(line, stop, ft_strlen(stop) + 1)))
			break ;
		if (ft_strchr(line, '$') && !(ft_strchr(limiter, '\'') \
		|| ft_strchr(limiter, '\"')))
			line = expand_hdoc(line);
		ft_putendl_fd(line, hdoc[WRITE]);
		free(line);
	}
	if (!line)
		error_msg("warning: here-document by end-of-file (wanted `", \
			stop, "'", 0);
	free(stop);
	free(line);
	if (minishell->pipe)
		free_pipes(minishell);
	ft_exit(minishell, NULL, 0);
}

/* Creates fork to execute here doc and sends to execution */
void	here_doc(t_commands *cmd, char *limiter, t_minishell *minishell)
{
	int	hdoc[2];
	int	pid;

	pipe(hdoc);
	signal(SIGINT, hdoc_handle_c);
	pid = fork();
	if (!pid)
		do_hdoc(hdoc, limiter, minishell);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &g_exit.status, 0);
	if (WIFEXITED(g_exit.status))
		g_exit.status = WEXITSTATUS(g_exit.status);
	init_signals();
	cmd->fd_in = dup(hdoc[READ]);
	dup2(g_exit.minishell->fd_in, STDIN_FILENO);
	dup2(g_exit.minishell->fd_out, STDOUT_FILENO);
	if (close(hdoc[READ]) < 0)
		perror("minishell: close hdoc[READ]");
	if (close(hdoc[WRITE]) < 0)
		perror("minishell: close hdoc[WRITE]");
}
