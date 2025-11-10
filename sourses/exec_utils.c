/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:10:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/11 02:19:52 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	handle_child_status(int status)
{
	setup_interactive_signals();
	handle_child_signal_exit(status);
	return (get_current_exit_status());
}

int	exec_builtin_no_redirections(t_command *cmd, t_env **env_list)
{
	exec_builtin(cmd, env_list);
	update_underscore_var(env_list, cmd->argc[0]);
	return (get_current_exit_status());
}

int	exec_builtin_with_fork(t_command *cmd, t_env **env_list)
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
		exec_builtin(cmd, env_list);
		exit(get_current_exit_status());
	}
	else if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	waitpid(pid, &status, 0);
	update_underscore_var(env_list, cmd->argc[0]);
	return (handle_child_status(status));
}

void	update_external_underscore(t_command *cmd, t_env **env_list)
{
	char	*executable;

	executable = find_executable_in_path(cmd->argc[0], *env_list);
	if (executable)
	{
		update_underscore_var(env_list, executable);
		free(executable);
	}
}
