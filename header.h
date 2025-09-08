#ifndef HEADER_H
# define HEADER_H

# include "./Libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

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
}				t_toktype;

typedef struct s_token
{
	char		*str;
	t_toktype	type;
}				t_token;

// my_shell.c
void			exec_command(char *cmd, char **envp);

// utilc.c
int				ft_strcmp(const char *s1, const char *s2);
size_t			ft_strlen(const char *s);
int				ft_atoi(const char *str);
int				err_message(const char *m1, const char *m2, const char *m3);

// tokenize.c
int				if_invalid_input(t_token *t, int count, int i);
t_token			*tokenisation(char **args, int count);
#endif
