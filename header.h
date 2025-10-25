#ifndef HEADER_H
# define HEADER_H
# define PATH_MAX 4096

# include "./Libft/libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_signal_received;
extern int						rl_on_new_line(void);
extern void						rl_redisplay(void);
# define SET_EXIT_STATUS(x) set_exit_status(x)
# define GET_EXIT_STATUS() get_current_exit_status()

typedef enum e_toktype
{
	T_PIPE,
	T_IN_REDIR,
	T_OUT_REDIR,
	T_APPEND_REDIR,
	T_HEREDOC,
	T_IN_FILE,
	T_OUT_FILE,
	T_APPEND_FILE,
	T_LIMITER,
	T_WORD
}								t_toktype;

typedef struct s_token
{
	char						*str;
	t_toktype					type;
	int							quote_type;
}								t_token;

typedef struct s_env
{
	char						*key;
	char						*value;
	struct s_env				*next;
}								t_env;

typedef struct s_redir
{
	char						*filename;
	int							append;
	struct s_redir				*next;
}								t_redir;

typedef struct s_command
{
	char						**argc;
	char						*input;
	t_redir						*output_list;
	struct s_command			*next;
}								t_command;

int								ft_env_lstsize(t_env *lst);
char							*ft_strjoin_env(const char *s1, const char *s2,
									char sep);
char							**env_to_array(t_env *env_list);
char							**list_to_array(t_env *env_list);
void							print_env(char **env);
void							add_env_node(t_env **env_list, char *env_line);
void							free_env_list(t_env *env_list);
t_env							*init_env(char **envp);

t_toktype						find_type(char *str, t_token *tokens, int i);
t_token							*tokenisation(char *input, int *out_count);
int								if_invalid_input(t_token *t, int count);
int								count_tokens(char *line);
char							**ft_split_input(char *line);
void							free_tokens(t_token *tokens, int count);
void							expand_dollar_vars(t_token *tokens, int count,
									t_env *env_list);

int								handle_heredoc(char *limiter);
void							process_heredocs(t_token *tokens, int count);

int								setup_redirections(t_command *cmd);
void							exec_command_with_redirections(t_command *cmd,
									char **envp);
t_redir							*add_output_redir(t_redir **head,
									char *filename, int append);
void							free_redir_list(t_redir *head);

t_command						*parse_tokens(t_token *tokens, int count);
void							free_cmd_list(t_command *cmd_list);
void							exec_cmd_list(t_command *cmd_list, char **env);
void							exec_command(char **cmd, char **envp);
int								execute_pipeline(t_command *cmd_list,
									t_env **env_list);

int								ft_strcmp(const char *s1, const char *s2);
size_t							ft_strlen(const char *s);
int								ft_atoi(const char *str);
int								err_message(const char *m1, const char *m2,
									const char *m3);

int								is_builtin(t_command *cmd);
void							exec_builtin(t_command *cmd, t_env **env);
void							ft_echo(char **argc);
void							ft_cd(t_command *cmd, t_env *env);
void							ft_pwd(void);
void							ft_export(t_env **env, char **argc);
void							ft_unset(t_env **env, char **argc);
void							ft_env(t_env *env);
void							ft_exit(t_command *cmd);

void							setup_interactive_signals(void);
void							setup_execution_signals(void);
int								check_signal_status(void);
void							set_exit_status(int status);
int								get_current_exit_status(void);

#endif