/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 13:05:30 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/13 15:33:27 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	write_heredoc_line(int pipe_fd, char *processed_line)
{
	char	*line_with_newline;

	line_with_newline = ft_strjoin(processed_line, "\n");
	if (line_with_newline)
	{
		write(pipe_fd, line_with_newline, ft_strlen(line_with_newline));
		free(line_with_newline);
	}
}

void	process_heredoc_line(int pipe_fd, char *str, int should_expand,
		t_env *env_list)
{
	char	*processed_line;

	if (should_expand)
		processed_line = expand_heredoc_line(str, env_list);
	else
		processed_line = ft_strdup(str);
	if (!processed_line)
		return ;
	write_heredoc_line(pipe_fd, processed_line);
	free(processed_line);
}

void	handle_eof_warning(char *limiter)
{
	ft_putstr_fd("minishell: warning: here-document ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(limiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

static int	read_line_from_stdin(char **out)
{
	char	c;
	ssize_t	r;

	*out = ft_strdup("");
	if (!*out)
		return (-1);
	r = read(STDIN_FILENO, &c, 1);
	while (r > 0 && c != '\n')
	{
		append_char(out, c);
		r = read(STDIN_FILENO, &c, 1);
	}
	if (r <= 0 && ft_strlen(*out) == 0)
	{
		free(*out);
		*out = NULL;
		return (-1);
	}
	return (0);
}

int	read_and_process_line(int pipe_fd, char *limiter, int should_expand,
		t_env *env_list)
{
	char	*str;
	char	*linebuf;

	if (isatty(STDIN_FILENO))
	{
		str = readline("> ");
		if (!str)
		{
			handle_eof_warning(limiter);
			return (1);
		}
		if (ft_strcmp(str, limiter) == 0)
		{
			free(str);
			return (1);
		}
		process_heredoc_line(pipe_fd, str, should_expand, env_list);
		free(str);
		return (0);
	}
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

void	run_heredoc(int pipe_fd, char *limiter, int should_expand,
		t_env *env_list)
{
	while (1)
	{
		if (read_and_process_line(pipe_fd, limiter, should_expand, env_list))
			break ;
	}
}
