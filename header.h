/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 21:26:17 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/24 15:12:13 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H
# define PATH_MAX 4096

# include "./Libft/libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern int					g_signal_received;

typedef enum e_toktype
{
	T_PIPE,
	T_AND,
	T_OR,
	T_IN_REDIR,
	T_OUT_REDIR,
	T_APPEND_REDIR,
	T_HEREDOC,
	T_IN_FILE,
	T_OUT_FILE,
	T_APPEND_FILE,
	T_LIMITER,
	T_WORD
}							t_toktype;

typedef struct s_token
{
	char					*str;
	t_toktype				type;
	int						quote_type;
}							t_token;

typedef struct s_env
{
	char					*key;
	char					*value;
	struct s_env			*next;
}							t_env;

typedef struct s_redir
{
	char					*filename;
	int						append;
	int						type;
	int						should_expand;
	struct s_redir			*next;
}							t_redir;

typedef struct s_tokenizer
{
	t_token					*tokens;
	char					*buf;
	int						count;
	int						i;
	int						sq;
	int						dq;
	int						quote_type;
}							t_tokenizer;

typedef struct s_input_redir
{
	char					*filename;
	struct s_input_redir	*next;
}							t_input_redir;

typedef enum e_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND
}							t_redir_type;

typedef struct s_ordered_redir
{
	t_redir_type			type;
	char					*filename;
	int						order;
	struct s_ordered_redir	*next;
}							t_ordered_redir;

typedef struct s_command
{
	char					**argc;
	char					*input;
	t_input_redir			*input_list;
	t_redir					*output_list;
	t_ordered_redir			*ordered_redirs;
	struct s_command		*next;
}							t_command;

typedef struct s_append_data
{
	t_command				**curr;
	t_command				**head;
	t_token					*tokens;
	int						start;
	int						end;
}							t_append_data;

typedef struct s_pipe_ctx
{
	int						**pipes;
	int						cmd_count;
	t_env					**env_list;
}							t_pipe_ctx;

int							ft_env_lstsize(t_env *lst);
char						*ft_strjoin_env(const char *s1, const char *s2,
								char sep);
char						**env_to_array(t_env *env_list);
void						print_env(char **env);
void						add_env_node(t_env **env_list, char *env_line);
void						free_env_list(t_env *env_list);
void						free_string_array(char **array);
t_env						*init_env(char **envp);
t_toktype					find_type(char *str, t_token *tokens, int i);
t_token						*tokenisation(char *input, int *out_count);
int							if_invalid_input(t_token *t, int count);
int							count_tokens(char *line);
char						**ft_split_input(char *line);
void						free_tokens(t_token *tokens, int count);
void						expand_dollar_vars(t_token *tokens, int count,
								t_env *env_list);
int							is_special_char(char c);
void						append_char(char **buf, char c);
void						add_token_with_quotes(t_token **tokens, int *count,
								char *str, int quote_type);
void						add_token(t_token **tokens, int *count, char *str);
void						init_tokenizer(t_tokenizer *tok);
void						flush_buffer(t_tokenizer *tok);
void						handle_special_chars(t_tokenizer *tok, char *input);
void						handle_quotes(t_tokenizer *tok, char c);

int							handle_heredoc(char *limiter, int should_expand,
								t_env *env_list);
int							process_heredocs(t_token *tokens, int count,
								t_env *env_list);
void						copy_strings(char *dest, char *s1, char *s2);
char						*ft_strjoin_heredoc(char *s1, char *s2);
char						*get_variable_value(char *var_name,
								t_env *env_list);
int							check_limiter(char *str, char *limiter);
int							handle_too_many_args(t_command *cmd);
int							setup_redirections(t_command *cmd);
int							validate_redir(t_input_redir *input_list);
int							setup_output_redirect(t_redir *redir_list);
int							setup_input_redirect(char *filename);
void						exec_cmd_helper(t_command *cmd, char **envp);
int							validate_helper(t_ordered_redir *redirs);
int							process_single_redir(t_ordered_redir *redir);
int							validate_setup_helper(t_ordered_redir *redirs);
int							setup_helper(t_ordered_redir *redirs);
void						cleanup_fds(int *fds, int count);
void						exec_cmd(t_command *cmd, char **envp);
t_redir						*add_output_redir(t_redir **head, char *filename,
								int append);
void						free_redir_list(t_redir *head);
t_input_redir				*add_input_redir(t_input_redir **head,
								char *filename);
void						free_input_redir_list(t_input_redir *head);
t_ordered_redir				*add_ordered_redir(t_ordered_redir **head,
								t_redir_type type, char *filename, int order);
void						free_ordered_redir_list(t_ordered_redir *head);
int							setup_ordered_redirections(t_command *cmd);
int							handle_input_redir(t_command *cmd, t_token *token,
								int pos);
int							handle_output_redir(t_command *cmd, t_token *token,
								int pos);
int							handle_append_redir(t_command *cmd, t_token *token,
								int pos);
int							count_argc(t_token *tokens, int start, int end);
void						init_command(t_command *cmd);
int							fill_command(t_command *cmd, t_token *tokens,
								int start, int end);
t_command					*parse_tokens(t_token *tokens, int count);
void						free_cmd_list(t_command *cmd_list);
void						exec_cmd_list(t_command *cmd_list, char **env);
void						exec_command(char **cmd, char **envp);
int							execute_pipeline(t_command *cmd_list,
								t_env **env_list);
int							count_commands(t_command *cmd_list);
void						exec_child(t_command *cmd, t_pipe_ctx *ctx, int i);
void						close_all_pipes(int **pipes, int cmd_count);
int							setup_pipes(int **pipes, int cmd_count);
void						manage_pipes_cleanup(int **pipes, int cmd_count,
								int free_mem);
void						setup_child_io(int **pipes, int i, int cmd_count);
void						execute_child_pipeline(t_command *current,
								t_pipe_ctx *ctx, int i);
int							wait_for_children(pid_t *pids, int cmd_count);
char						*find_executable_in_path(char *cmd,
								t_env *env_list);
char						*find_executable_in_envp(char *cmd, char **envp);

int							ft_strcmp(const char *s1, const char *s2);
size_t						ft_strlen(const char *s);
int							ft_atoi(const char *str);
int							err_message(const char *m1, const char *m2,
								const char *m3);
int							is_builtin(t_command *cmd);
void						exec_builtin(t_command *cmd, t_env **env);
void						ft_echo(char **argc);
void						ft_cd(t_command *cmd, t_env *env);
void						ft_pwd(void);
void						ft_export(t_env **env, char **argc);
void						ft_unset(t_env **env, char **argc);
void						ft_env(t_env *env);
void						ft_exit(t_command *cmd, t_env **env_list);
void						setup_interactive_signals(void);
void						setup_execution_signals(void);
void						setup_heredoc_signals(void);
int							check_signal_status(void);
void						set_exit_status(int status);
int							get_current_exit_status(void);
void						setup_child_signals(void);
void						handle_child_signal_exit(int status);
void						sigint_heredoc(int sig);
t_ordered_redir				*new_ordered_redir(t_redir_type type,
								char *filename, int order);
char						*get_env_value(t_env *env_list, char *key);
char						*get_var_name(char *str, int start, int *end);
char						*replace_dollar_var(char *str, int start, int end,
								char *replacement);
void						expand_dollar_vars(t_token *tokens, int count,
								t_env *env_list);
int							export_variable(t_env **env, char *key, char *val);
int							process_export_arg(t_env **env, char *arg);
int							is_valid_identifier(char *str);
int							is_numeric(char *str);
int							update_existing_var(t_env *cur, char *val);
char						*duplicate_value(char *val);
int							init_env_strings(t_env *new, char *key, char *val);
t_env						*create_new_env(char *key, char *val);
void						print_exported_vars(t_env *env);
void						toggle_quotes(char c, int *sq, int *dq);
int							is_forbidden_char(char c);
int							report_syntax_error(void);
int							has_syntax_errors(char *str);
int							handle_syntax_check(char *line);
t_token						*tokenize_and_validate(char *line,
								int *token_count);
t_command					*prepare_commands(t_token *tokens, int token_count,
								t_env *env_list, int *original_stdin);
void						restore_stdin(int original_stdin);
int							handle_input(char *line, t_env **env_list);
int							has_unclosed_quotes(char *str);
int							handle_eof_error(char *full_line);
char						*append_continuation(char *full_line,
								char *continuation);
char						*read_continuation_line(char *full_line,
								int is_interactive);
int							**create_pipes(int pipe_count);
void						cleanup_pipes(int **pipes, int pipe_count);
int							wait_and_cleanup(int **pipes, pid_t *pids,
								int cmd_count);
int							cleanup_and_error(int **pipes, pid_t *pids,
								int pipe_count);
int							fork_error(int **pipes, pid_t *pids,
								int pipe_count);
void						exec_external(t_command *cmd, t_env **env);
int							exec_single_command(t_command *cmd,
								t_env **env_list);
int							handle_empty_command(t_command *cmd);
int							exec_external_single(t_command *cmd,
								t_env **env_list);
void						exec_child_single(t_command *cmd, char **env_array);
void						update_underscore_var(t_env **env_list,
								char *cmd_path);
int							exec_builtin_no_redirections(t_command *cmd,
								t_env **env_list);
int							exec_builtin_with_fork(t_command *cmd,
								t_env **env_list);
void						update_external_underscore(t_command *cmd,
								t_env **env_list);
int							handle_child_status(int status);
int							process_dollar_variable(char **new_line, int i,
								t_env *env_list);
char						*expand_heredoc_line(char *line, t_env *env_list);
int							handle_interactive_heredoc(int pipe_fd,
								char *limiter, int should_expand,
								t_env *env_list);
int							read_line_from_stdin(char **out);
void						write_heredoc_line(int pipe_fd,
								char *processed_line);
void						process_heredoc_line(int pipe_fd, char *str,
								int should_expand, t_env *env_list);
void						handle_eof_warning(char *limiter);
int							read_and_process_line(int pipe_fd, char *limiter,
								int should_expand, t_env *env_list);
void						run_heredoc(int pipe_fd, char *limiter,
								int should_expand, t_env *env_list);
void						heredoc_child(int pipe_fd[2], char *limiter,
								int should_expand, t_env *env_list);
int							handle_parent_process(pid_t pid, int pipe_fd[2]);
int							handle_fork_error(int pipe_fd[2]);
int							handle_heredoc(char *limiter, int should_expand,
								t_env *env_list);
int							collect_heredoc_fd(t_token *tokens, int i,
								t_env *env_list, int *last_fd);
long						ft_atol(const char *str);

#endif
