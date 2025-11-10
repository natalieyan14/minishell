/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 20:31:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/11 02:19:29 by natalieyan       ###   ########.fr       */
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
