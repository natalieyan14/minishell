/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:06:09 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/30 02:36:44 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	setup_input(char *filename)
{
	int	fd;

	if (!filename)
		return (0);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror(filename);
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

static int	setup_output(t_redir *redir_list)
{
	return (setup_output_redirect(redir_list));
}

int	setup_input_redirect(char *filename)
{
	return (setup_input(filename));
}

int	validate_redir(t_input_redir *input_list)
{
	t_input_redir	*curr;
	int				fd;

	if (!input_list)
		return (0);
	curr = input_list;
	while (curr)
	{
		fd = open(curr->filename, O_RDONLY);
		if (fd < 0)
		{
			perror(curr->filename);
			set_exit_status(1);
			return (-1);
		}
		close(fd);
		curr = curr->next;
	}
	return (0);
}

int	setup_redirections(t_command *cmd)
{
	int	input_result;
	int	output_result;

	output_result = setup_output(cmd->output_list);
	if (output_result < 0)
		return (-1);
	input_result = setup_input(cmd->input);
	if (input_result < 0)
		return (-1);
	return (0);
}
