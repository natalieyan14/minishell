/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 16:12:57 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/11 16:13:38 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:00:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/11 15:00:00 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header.h"

static int	validate_output_redir(t_ordered_redir *curr)
{
	int	fd;
	int	flags;

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
	return (0);
}

static int	validate_input_redir(t_ordered_redir *curr)
{
	int	fd;

	fd = open(curr->filename, O_RDONLY);
	if (fd < 0)
	{
		perror(curr->filename);
		set_exit_status(1);
		return (-1);
	}
	close(fd);
	return (0);
}

int	validate_setup_helper(t_ordered_redir *redirs)
{
	t_ordered_redir	*curr;

	curr = redirs;
	while (curr)
	{
		if (curr->type == REDIR_INPUT)
		{
			if (validate_input_redir(curr) < 0)
				return (-1);
		}
		else
		{
			if (validate_output_redir(curr) < 0)
				return (-1);
		}
		curr = curr->next;
	}
	return (0);
}

static int	setup_input_redir(t_ordered_redir *curr)
{
	int	fd;

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
	return (0);
}

int	setup_helper(t_ordered_redir *redirs)
{
	t_ordered_redir	*curr;

	curr = redirs;
	while (curr)
	{
		if (curr->type == REDIR_INPUT)
		{
			if (setup_input_redir(curr) < 0)
				return (-1);
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
