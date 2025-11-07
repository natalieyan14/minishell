/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 01:50:33 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/07 19:35:53 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header.h"

static void	exec_child_process(t_command *cmd, char **envp)
{
	char	*executable;
	int		output_result;
	int		input_result;

	output_result = setup_output_redirect(cmd->output_list);
	if (output_result < 0)
		exit(1);
	input_result = setup_input_redirect(cmd->input);
	if (input_result < 0)
		exit(1);
	executable = find_executable_in_envp(cmd->argc[0], envp);
	if (!executable)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argc[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	if (execve(executable, cmd->argc, envp) == -1)
	{
		perror("execve failed");
		exit(127);
	}
}

static void	handle_parent_process(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		set_exit_status(WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		set_exit_status(128 + WTERMSIG(status));
}

int	setup_ordered_redirections(t_command *cmd)
{
	if (!cmd->ordered_redirs)
		return (setup_redirections(cmd));
	if (validate_helper(cmd->ordered_redirs) < 0)
		return (-1);
	if (setup_helper(cmd->ordered_redirs) < 0)
		return (-1);
	return (0);
}

void	exec_cmd_helper(t_command *cmd, char **envp)
{
	pid_t	pid;

	if (!cmd || !cmd->argc || !cmd->argc[0])
		return ;
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		set_exit_status(1);
		return ;
	}
	else if (pid == 0)
		exec_child_process(cmd, envp);
	else
		handle_parent_process(pid);
}
