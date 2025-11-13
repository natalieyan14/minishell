/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 01:50:40 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/11 16:12:05 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header.h"

static int	process_output_redir(t_ordered_redir *redir)
{
	int	fd;
	int	flags;

	flags = O_CREAT | O_WRONLY;
	if (redir->type == REDIR_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(redir->filename, flags, 0644);
	if (fd < 0)
	{
		perror(redir->filename);
		set_exit_status(1);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		set_exit_status(1);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	process_input_redir(t_ordered_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->filename);
		set_exit_status(1);
		return (-1);
	}
	close(fd);
	return (0);
}

int	process_single_redir(t_ordered_redir *redir)
{
	if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
		return (process_output_redir(redir));
	else if (redir->type == REDIR_INPUT)
		return (process_input_redir(redir));
	return (0);
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
