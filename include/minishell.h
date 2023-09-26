/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 11:17:01 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/15 15:40:56 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdbool.h>
# include <signal.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/stat.h>

# define BLUE "\001\x1b[34;40;1m\002"
# define YELLOW "\001\x1b[33;40;1;3m\002"
# define CYAN "\001\x1b[36;40;1m\002"
# define GREEN "\001\x1b[32;40;1;3m\002"
# define RESET "\001\x1b[0m\002"

enum	e_pipe
{
	READ,
	WRITE
};
typedef enum e_type
{
	WORD,
	VAR,
	DQUOTE = '"',
	SQUOTE = '\'',
	PIPE = '|',
	REDIR_IN = '<',
	REDIR_OUT = '>',
	ENV = '$',
	HEREDOC,
	DBLREDIR_OUT,
}	t_type;

typedef struct s_elements
{
	char		*element;
	t_type		type;
	bool		env;
}	t_element;

typedef struct redirect
{
	t_type	type;
	char	*file;
}	t_redirect;

/* each command has the full prompt, but also an array of structs with
information about each piece of the command; */
typedef struct s_commands
{
	char	**args;
	char	*cmd_path;
	int		nbr_args;
	int		fd_in;
	int		fd_out;
	t_list	*redirects;
	t_list	*vars;
}	t_commands;

/* env vars and values stored as VAR_NAME and VAR_VALUE
(clean) e.g. USER | heda-sil */
typedef struct s_env
{
	char	*name;
	char	*value;
	bool	exported;
	bool	system;
}	t_env;

typedef struct s_minishell
{
	char		*full_prompt;
	t_list		*envp;
	t_element	**token;
	t_commands	**commands;
	int			nbr_tokens;
	int			nbr_commands;
	int			nbr_pipes;
	char		*home;
	int			fd_in;
	int			fd_out;
	int			cmd_idx;
	int			**pipe;
}	t_minishell;

typedef struct s_exit
{
	int			status;
	t_minishell	*minishell;
}				t_exit;

extern t_exit	g_exit;

typedef struct s_lexer_helper
{
	int		words;
	int		counter;
}	t_lexer_helper;

// ###BUILT-INS###
void		ft_cd(t_commands *cmd, t_list *envp, t_minishell *minishell);
void		ft_echo(t_commands *cmd, bool n_check, bool newline, int i);
//ENV
char		**env_to_array(t_list *envp);
void		ft_env(t_commands *cmd, t_list *envp);
t_list		*duplicate_envp(char **envp, char **home);
char		*find_in_env(char *var, t_list *env);
//EXPORT
void		ft_export(t_commands *cmd, t_list **envp, int export);
void		ft_export_no_args(t_list **envp);
bool		export_name_validation(char *key, int print);
t_list		*ft_copy_list(t_list **envp);
void		export_order(t_list **exp);
bool		export_check(t_list **envp, char **var, char *token, int export);
void		add_export(t_list **envp, char *cmd, char *name, int export);
//UNSET
void		remove_node(t_list **envp, t_list *prev, t_list *tmp);
void		ft_unset(t_list **envp, t_commands *cmd);
//EXIT
void		ft_exit(t_minishell *minishell, t_commands *cmd, int print);
void		error_msg(char *msg, char *arg1, char *arg2, int error);

//EXECUTOR
void		exec_built_ins(t_minishell *minishell, t_commands *cmd);
pid_t		forking(t_minishell *minishell);
void		exec(t_minishell *minishell, int idx);
void		executor_main(t_minishell *minishell);

//EXECUTOR_UTILS
int			is_built_in(char *cmd);
void		handle_vars(t_commands *cmd, t_list *envp);
void		handle_redirects(t_commands *cmd);

//EXEC_CMD
char		**get_path(t_list *envp);
char		*get_cmd_path(t_list *envp, char *cmd);
void		exec_errors(char *cmd_path, char *args);
void		exec_cmd(t_minishell *minishell, t_commands *cmd);

//EXPANSOR
char		*get_vars(char *var, t_list *env, size_t size, int *j);
char		*expand_vars(char *token, t_list *envp, int *j);
char		*expand_token(char *token, int *j, t_list *env, int expand);
char		*expansor(char *element, int *j, t_list *env, int *quote);
void		expansor_main(t_element **tokens, t_list *envp);

//SIGNALS
void		init_signals(void);
void		handle_c(int signal);
void		handle_child(int signal);
void		hdoc_handle_c(int signal);

//LEXER
void		lexer(t_minishell *minishell);
bool		verify_quotes(t_minishell *minishell);

//LEXER_UTILS
int			skip_quotes(const char *str, int i, char c);
int			redirect_size(const char *s, int i);
void		redirect_tokens(const char *s, t_lexer_helper *lh);
void		set_token_type(t_element **tokens);

//PARSER_UTILS
bool		syntax_check(t_element **token);
int			count_cmd_args(t_element **token);
void		count_forks(t_minishell *minishell, t_element **token);

//PARSER
t_commands	*creat_cmd(t_commands *cmd, t_element **token);
void		clean_redirs(t_element **token, t_commands *cmd, int *k, int *j);
t_commands	**extract_cmds(t_minishell *minishell, t_element **token);
void		parser(t_minishell *minishell, t_element **token);

//PARSER
t_commands	*creat_cmd(t_commands *cmd, t_element **token);
void		clean_redirs(t_element **token, t_commands *cmd, int *k, int *j);
t_commands	**extract_cmds(t_minishell *minishell, t_element **token);
void		parser(t_minishell *minishell, t_element **token);

//REDIRECTS
void		redirects(t_commands *cmd, t_type redir, char *file);
int			limiter_size(char *limiter);
void		here_doc(t_commands *cmd, char *limiter, t_minishell *minishell);
t_redirect	*create_redirect(t_type redir, char *file);

//SPLIT_TOKENS
void		quote_time(const char *s, t_lexer_helper	*lhp);
void		ft_token_count(char *s, t_lexer_helper	*lexer_helper);
int			ft_word_size(char *s, int i, t_lexer_helper	*lexer_helper);
void		tokens_fill(t_element **tokens, char *s, t_lexer_helper *lexer_ptr);
t_element	**split_to_tokens(char *tokens);

//PIPES
int			**open_pipes(int **pipes, int nbr_pipes);
void		free_pipes(t_minishell *minishell);
void		set_pipes(t_commands *cmd, int *pipe[2], int idx, int nbr_cmds);
void		close_pipes(int i, int *pipe[2], int nbr_cmds);
void		dupping(t_commands **cmd, int idx);

//UTILS
void		del(void *content);
void		*clean_cmds(t_commands **cmds);
void		*clean_tokens(t_element **token);
void		clean_env(t_list **envp);
void		*clean_all(t_minishell *minishell, int exit);

//MAIN
void		fill_minishell(t_minishell *minishell, char *prompt);
#endif
