/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 20:30:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/10 23:06:21 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	exec_external(t_command *cmd, t_env **env_list)
{
	char	*executable;
	char	**envp;
	int		exit_code;

	if (!cmd || !cmd->argc || !cmd->argc[0])
		exit(127);
	envp = env_to_array(*env_list);
	executable = find_executable_in_path(cmd->argc[0], *env_list);
	if (!executable)
	{
		free_string_array(envp);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argc[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	if (execve(executable, cmd->argc, envp) == -1)
	{
		exit_code = 127;
		if (errno == EACCES)
			exit_code = 126;
		free_string_array(envp);
		free(executable);
		perror("execve failed");
		exit(exit_code);
	}
}
