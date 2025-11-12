/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 01:50:40 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/11 17:14:18 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header.h"

static int	exit_with_status_code(char *str, int code)
{
	perror(str);
	set_exit_status(code);
	return (-1);
}

static int	validate_setup_helper2(t_ordered_redir *curr)
{
	int	fd;

	fd = open(curr->filename, O_RDONLY);
	if (fd < 0)
		return (exit_with_status_code(curr->filename, 1));
	close(fd);
	return (fd);
}

int	validate_helper(t_ordered_redir *redirs)
{
	t_ordered_redir	*curr;
	int				fd;

	curr = redirs;
	while (curr)
	{
		if (curr->type == REDIR_INPUT)
		{
			fd = open(curr->filename, O_RDONLY);
			if (fd < 0)
			{
				perror(curr->filename);
				set_exit_status(1);
				return (-1);
			}
			close(fd);
		}
		curr = curr->next;
	}
	return (0);
}

int	validate_setup_helper(t_ordered_redir *redirs)
{
	t_ordered_redir	*curr;
	int				fd;
	int				flags;

	curr = redirs;
	while (curr)
	{
		if (curr->type == REDIR_INPUT && validate_setup_helper2(curr) == -1)
			return (-1);
		else
		{
			flags = O_CREAT | O_WRONLY;
			if (curr->type == REDIR_APPEND)
				flags |= O_APPEND;
			else
				flags |= O_TRUNC;
			fd = open(curr->filename, flags, 0644);
			if (fd < 0)
				return (exit_with_status_code(curr->filename, 1), -1);
			close(fd);
		}
		curr = curr->next;
	}
	return (0);
}

int	setup_helper(t_ordered_redir *redirs)
{
	t_ordered_redir	*curr;
	int				fd;

	curr = redirs;
	while (curr)
	{
		if (curr->type == REDIR_INPUT)
		{
			fd = open(curr->filename, O_RDONLY);
			if (fd < 0)
				return (exit_with_status_code(curr->filename, 1), -1);
			if (dup2(fd, STDIN_FILENO) < 0)
				return (close (fd), exit_with_status_code("dup2", 1), -1);
			close(fd);
		}
		else if (curr->type == REDIR_OUTPUT || curr->type == REDIR_APPEND)
		{
			if (process_single_redir(curr) < 0)
				return (-1);
		}
		curr = curr->next;
	}
	return (0);
}

void	cleanup_fds(int *fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(fds[i]);
		i++;
	}
	free(fds);
}
