/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 11:33:33 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/15 15:46:08 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirect	*create_redirect(t_type redir, char *file)
{
	t_redirect	*redirect;

	redirect = ft_calloc(1, sizeof(*redirect));
	redirect->type = redir;
	redirect->file = file;
	return (redirect);
}

void	set_redir_fds(int new_fd, int *old_fd, int std_fd, char *file)
{
	if (new_fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(file);
		g_exit.status = 1;
	}
	if (*old_fd > std_fd)
		close(*old_fd);
	*old_fd = new_fd;
}

// Stores the redirects fds
void	redirects(t_commands *cmd, t_type redir, char *file)
{
	int	fd;

	if (redir == REDIR_IN)
	{
		fd = open(file, O_RDONLY);
		set_redir_fds(fd, &cmd->fd_in, STDIN_FILENO, file);
	}
	if (redir == REDIR_OUT)
	{
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 00644);
		set_redir_fds(fd, &cmd->fd_out, STDOUT_FILENO, file);
	}
	if (redir == DBLREDIR_OUT)
	{
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 00644);
		set_redir_fds(fd, &cmd->fd_out, STDOUT_FILENO, file);
	}
	if (redir == HEREDOC)
		here_doc(cmd, file, g_exit.minishell);
}

int	limiter_size(char *limiter)
{
	int		i;
	int		j;
	char	control;

	i = 0;
	j = 0;
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
		i++;
		j++;
	}
	return (j);
}
