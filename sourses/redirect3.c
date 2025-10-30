/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 01:50:10 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/30 04:07:02 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	count_redirections(t_redir *redir_list)
{
	t_redir	*curr;
	int		count;

	count = 0;
	curr = redir_list;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

static int	open_single_file(t_redir *curr)
{
	int	fd;
	int	flags;

	flags = O_CREAT | O_WRONLY;
	if (curr->append)
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
	return (fd);
}

static int	open_output_files(t_redir *redir_list, int *fds)
{
	t_redir	*curr;
	int		fd;
	int		i;
	int		last_valid_fd;

	i = 0;
	last_valid_fd = -1;
	curr = redir_list;
	while (curr)
	{
		fd = open_single_file(curr);
		if (fd >= 0)
		{
			fds[i] = fd;
			last_valid_fd = i;
			i++;
		}
		curr = curr->next;
	}
	if (last_valid_fd == -1)
		return (-1);
	return (i);
}

static int	finalize_output(int *fds, int count)
{
	int	i;

	i = 0;
	while (i < count - 1)
	{
		close(fds[i]);
		i++;
	}
	if (dup2(fds[count - 1], STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fds[count - 1]);
		free(fds);
		set_exit_status(1);
		return (-1);
	}
	close(fds[count - 1]);
	free(fds);
	return (0);
}

int	setup_output_redirect(t_redir *redir_list)
{
	int	*fds;
	int	max_count;
	int	actual_count;

	if (!redir_list)
		return (0);
	max_count = count_redirections(redir_list);
	fds = malloc(sizeof(int) * max_count);
	if (!fds)
		return (-1);
	actual_count = open_output_files(redir_list, fds);
	if (actual_count < 0)
		return (-1);
	return (finalize_output(fds, actual_count));
}
