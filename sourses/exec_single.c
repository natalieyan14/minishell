/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:57:52 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/25 12:22:58 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	handle_single_builtin(t_command *cmd, t_env **env_list)
{
	if (!cmd->input_list && !cmd->output_list && !cmd->ordered_redirs)
		return (exec_builtin_no_redirections(cmd, env_list));
	return (exec_builtin_with_fork(cmd, env_list));
}

static int	handle_single_external(t_command *cmd, t_env **env_list)
{
	pid_t	pid;
	int		status;

	setup_execution_signals();
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		if (setup_ordered_redirections(cmd) < 0)
			exit(1);
		exec_external(cmd, env_list);
	}
	else if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	waitpid(pid, &status, 0);
	update_external_underscore(cmd, env_list);
	return (handle_child_status(status));
}

static int	handle_output_files(t_redir *output_list)
{
	t_redir	*curr;
	int		fd;
	int		flags;

	curr = output_list;
	while (curr)
	{
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
			return (1);
		}
		close(fd);
		curr = curr->next;
	}
	return (0);
}

int	handle_empty_command(t_command *cmd)
{
	if (cmd->ordered_redirs)
	{
		if (validate_setup_helper(cmd->ordered_redirs) < 0)
			return (1);
		return (0);
	}
	if (cmd->input_list)
	{
		if (validate_redir(cmd->input_list) < 0)
			return (1);
	}
	if (cmd->output_list)
		return (handle_output_files(cmd->output_list));
	return (0);
}

int	exec_single_command(t_command *cmd, t_env **env_list)
{
	int	ret;

	if (!cmd)
		return (0);
	if (!cmd->argc || !cmd->argc[0])
		return (handle_empty_command(cmd));
	if (cmd->argc[0] && (ft_strcmp(cmd->argc[0], ".") == 0
			|| ft_strcmp(cmd->argc[0], "..") == 0
			|| ft_strcmp(cmd->argc[0], "") == 0))
	{
		if (ft_strcmp(cmd->argc[0], "") == 0)
			ft_putstr_fd("Command '' not found, but can be installed with:\n",
				STDERR_FILENO);
		else
		{
			ft_putstr_fd(cmd->argc[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
		}
		return (set_exit_status(127), 127);
	}
	if (is_builtin(cmd))
		ret = handle_single_builtin(cmd, env_list);
	else
		ret = handle_single_external(cmd, env_list);
	return (set_exit_status(ret), ret);
}
