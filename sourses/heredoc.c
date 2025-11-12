/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:19:54 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/11 10:09:25 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	process_dollar_variable(char **new_line, int i, t_env *env_list)
{
	char	*var_name;
	char	*var_value;
	int		var_end;
	int		new_i;

	var_name = get_var_name(*new_line, i, &var_end);
	if (var_name && ft_strlen(var_name) > 0)
	{
		var_value = get_variable_value(var_name, env_list);
		*new_line = replace_dollar_var(*new_line, i, var_end, var_value);
		new_i = i + ft_strlen(var_value);
		free(var_value);
		free(var_name);
		return (new_i);
	}
	if (var_name)
		free(var_name);
	return (i + 1);
}

static char	*expand_heredoc_line(char *line, t_env *env_list)
{
	char	*new_line;
	int		i;

	i = 0;
	new_line = ft_strdup(line);
	while (new_line && (size_t)i < ft_strlen(new_line))
	{
		if (new_line[i] == '$')
			i = process_dollar_variable(&new_line, i, env_list);
		else
			i++;
	}
	return (new_line);
}

static void	process_heredoc_line(int pipe_fd, char *str, int should_expand,
		t_env *env_list)
{
	char	*processed_line;
	char	*line_with_newline;

	if (should_expand)
		processed_line = expand_heredoc_line(str, env_list);
	else
		processed_line = ft_strdup(str);
	line_with_newline = ft_strjoin_heredoc(processed_line, "\n");
	if (line_with_newline)
	{
		write(pipe_fd, line_with_newline, ft_strlen(line_with_newline));
		free(line_with_newline);
	}
}

static void	run_heredoc(int pipe_fd, char *limiter, int should_expand,
		t_env *env_list)
{
	char	*str;

	while (1)
	{
		str = readline("> ");
		if (check_limiter(str, limiter))
		{
			if (str)
				free(str);
			break ;
		}
		process_heredoc_line(pipe_fd, str, should_expand, env_list);
		free(str);
	}
}

int	handle_heredoc(char *limiter, int should_expand, t_env *env_list)
{
	int	pipe_fd[2];
	int	original_stdin;

	if (!limiter)
		return (-1);
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe error");
		set_exit_status(1);
		return (-1);
	}
	original_stdin = dup(STDIN_FILENO);
	run_heredoc(pipe_fd[1], limiter, should_expand, env_list);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	return (original_stdin);
}
