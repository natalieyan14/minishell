/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 20:31:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/09 21:15:14 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	handle_child_status(int status)
{
	setup_interactive_signals();
	if (WIFEXITED(status))
		set_exit_status(WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		set_exit_status(128 + WTERMSIG(status));
	return (get_current_exit_status());
}

static int	handle_single_builtin(t_command *cmd, t_env **env_list)
{
	pid_t	pid;
	int		status;

	if (!cmd->input_list && !cmd->output_list && !cmd->ordered_redirs)
	{
		exec_builtin(cmd, env_list);
		return (get_current_exit_status());
	}
	setup_execution_signals();
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		if (setup_ordered_redirections(cmd) < 0)
			exit(1);
		exec_builtin(cmd, env_list);
		exit(get_current_exit_status());
	}
	else if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	waitpid(pid, &status, 0);
	return (handle_child_status(status));
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
	return (handle_child_status(status));
}

int	exec_single_command(t_command *cmd, t_env **env_list)
{
	if (!cmd)
		return (0);
	if (!cmd->argc || !cmd->argc[0] || ft_strlen(cmd->argc[0]) == 0)
		return (0);
	if (is_builtin(cmd))
		return (handle_single_builtin(cmd, env_list));
	return (handle_single_external(cmd, env_list));
}
