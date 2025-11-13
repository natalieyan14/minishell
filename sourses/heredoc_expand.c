/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:19:54 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/13 16:10:09 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	process_dollar_variable(char **new_line, int i, t_env *env_list)
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

char	*expand_heredoc_line(char *line, t_env *env_list)
{
	char	*new_line;
	int		i;

	i = 0;
	new_line = ft_strdup(line);
	if (!new_line)
		return (NULL);
	while (new_line && (size_t)i < ft_strlen(new_line))
	{
		if (new_line[i] == '$')
			i = process_dollar_variable(&new_line, i, env_list);
		else
			i++;
	}
	return (new_line);
}

static int	handle_stdin_heredoc(int pipe_fd, char *limiter, int should_expand,
		t_env *env_list)
{
	char	*linebuf;

	if (read_line_from_stdin(&linebuf) < 0)
	{
		handle_eof_warning(limiter);
		return (1);
	}
	if (ft_strcmp(linebuf, limiter) == 0)
	{
		free(linebuf);
		return (1);
	}
	process_heredoc_line(pipe_fd, linebuf, should_expand, env_list);
	free(linebuf);
	return (0);
}

int	read_and_process_line(int pipe_fd, char *limiter, int should_expand,
		t_env *env_list)
{
	if (isatty(STDIN_FILENO))
		return (handle_interactive_heredoc(pipe_fd, limiter, should_expand,
				env_list));
	return (handle_stdin_heredoc(pipe_fd, limiter, should_expand, env_list));
}

void	run_heredoc(int pipe_fd, char *limiter, int should_expand,
		t_env *env_list)
{
	while (1)
	{
		if (read_and_process_line(pipe_fd, limiter, should_expand, env_list))
			break ;
	}
}
