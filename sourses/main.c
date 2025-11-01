/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:05:17 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/02 03:53:31 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	has_syntax_errors(char *str)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (str[i])
	{
		if (str[i] == '\'' && dq == 0)
			sq = !sq;
		else if (str[i] == '"' && sq == 0)
			dq = !dq;
		else if (!sq && !dq)
		{
			if (str[i] == '(' || str[i] == ')' || str[i] == '{'
				|| str[i] == '}')
			{
				write(STDERR_FILENO,
					"minishell: syntax error near unexpected token\n", 46);
				set_exit_status(2);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static int	handle_input(char *line, t_env **env_list)
{
	t_token		*tokens;
	t_command	*cmd_list;
	int			token_count;
	int			original_stdin;

	if (has_syntax_errors(line))
	{
		free(line);
		return (0);
	}
	token_count = 0;
	tokens = tokenisation(line, &token_count);
	if (!tokens || if_invalid_input(tokens, token_count) < 0)
	{
		free(line);
		free_tokens(tokens, token_count);
		return (0);
	}
	expand_dollar_vars(tokens, token_count, *env_list);
	original_stdin = process_heredocs(tokens, token_count);
	cmd_list = parse_tokens(tokens, token_count);
	if (!cmd_list)
	{
		free_tokens(tokens, token_count);
		free(line);
		return (0);
	}
	execute_pipeline(cmd_list, env_list);
	if (original_stdin >= 0)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
	free_tokens(tokens, token_count);
	free_cmd_list(cmd_list);
	free(line);
	return (1);
}

static int	has_unclosed_quotes(char *str)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (str[i])
	{
		if (str[i] == '\'' && dq == 0)
			sq = !sq;
		else if (str[i] == '"' && sq == 0)
			dq = !dq;
		i++;
	}
	return (sq || dq);
}

static char	*read_multiline_input(void)
{
	char	*line;
	char	*full_line;
	char	*continuation;
	char	*tmp;
	int		is_interactive;

	line = readline("minishell$ ");
	if (!line)
		return (NULL);
	full_line = ft_strdup(line);
	free(line);
	is_interactive = isatty(STDIN_FILENO);
	while (has_unclosed_quotes(full_line))
	{
		if (!is_interactive)
		{
			write(STDERR_FILENO,
				"minishell: unexpected EOF while looking for matching quote\n",
				59);
			set_exit_status(2);
			free(full_line);
			return (NULL);
		}
		continuation = readline("> ");
		if (!continuation)
		{
			write(STDERR_FILENO,
				"minishell: unexpected EOF while looking for matching quote\n",
				59);
			set_exit_status(2);
			free(full_line);
			return (NULL);
		}
		tmp = ft_strjoin(full_line, "\n");
		free(full_line);
		full_line = ft_strjoin(tmp, continuation);
		free(tmp);
		free(continuation);
	}
	return (full_line);
}

static int	run_shell(t_env *env_list)
{
	char	*line;

	setup_interactive_signals();
	while (1)
	{
		line = read_multiline_input();
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				write(1, "exit\n", 5);
			break ;
		}
		if (*line)
		{
			add_history(line);
			handle_input(line, &env_list);
		}
		else
			free(line);
	}
	return (get_current_exit_status());
}

int	main(int argc, char **argv, char **envp)
{
	t_env *env_list;
	int exit_status;

	(void)argc;
	(void)argv;
	exit_status = 0;
	env_list = init_env(envp);
	exit_status = run_shell(env_list);
	free_env_list(env_list);
	return (exit_status);
}