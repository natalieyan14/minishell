/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 16:33:09 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/12 21:07:11 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	exec_command(char **cmd, char **envp)
{
	pid_t	pid;
	char	*executable;

	pid = fork();
	if (pid < 0)
		perror("fork failed");
	else if (pid == 0)
	{
		setup_child_signals();
		executable = find_executable_in_envp(cmd[0], envp);
		if (!executable)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd[0], STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			exit(127);
		}
		if (execve(executable, cmd, envp) == -1)
		{
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, NULL, 0);
}
