/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 01:50:40 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/11 13:27:27 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header.h"

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
		else
		{
			flags = O_CREAT | O_WRONLY;
			if (curr->type == REDIR_APPEND)
				flags |= O_APPEND;
			else
				flags |= O_TRUNC;
			fd = open(curr->filename, flags, 0644);
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
			{
				perror(curr->filename);
				set_exit_status(1);
				return (-1);
			}
			if (dup2(fd, STDIN_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				set_exit_status(1);
				return (-1);
			}
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
