/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_add.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 23:13:39 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/22 23:13:43 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	collect_heredoc_fd(t_token *tokens, int i, t_env *env_list, int *last_fd)
{
	int	fd;

	fd = handle_heredoc(tokens[i + 1].str, (tokens[i + 1].quote_type == 0),
			env_list);
	if (fd < 0 && check_signal_status() == 130)
		return (-130);
	if (fd < 0)
		return (i + 2);
	if (*last_fd >= 0)
		close(*last_fd);
	*last_fd = fd;
	return (i + 2);
}

int	apply_heredoc_fd(int fd, int *original_stdin)
{
	if (fd < 0)
		return (-1);
	if (*original_stdin < 0)
	{
		*original_stdin = dup(STDIN_FILENO);
		if (*original_stdin < 0)
		{
			close(fd);
			set_exit_status(1);
			return (-1);
		}
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		if (*original_stdin >= 0)
		{
			dup2(*original_stdin, STDIN_FILENO);
			close(*original_stdin);
		}
		set_exit_status(1);
		return (-1);
	}
	close(fd);
	return (0);
}

int	finalize_heredocs(int last_fd, int *original_stdin)
{
	if (last_fd >= 0)
	{
		if (apply_heredoc_fd(last_fd, original_stdin) < 0)
			return (-2);
		set_exit_status(0);
	}
	return (0);
}

int	process_heredocs(t_token *tokens, int count, t_env *env_list)
{
	int	i;
	int	original_stdin;
	int	fd;
	int	last_fd;

	i = 0;
	original_stdin = -1;
	last_fd = -1;
	while (i < count - 1)
	{
		if (tokens[i].type == T_HEREDOC && tokens[i + 1].type == T_LIMITER)
		{
			fd = collect_heredoc_fd(tokens, i, env_list, &last_fd);
			if (fd == -130)
				return (-2);
			i = fd;
		}
		else
			i++;
	}
	if (finalize_heredocs(last_fd, &original_stdin) < 0)
		return (-2);
	return (original_stdin);
}
