/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 16:33:09 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 16:33:10 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	exec_command(char **cmd, char **envp)
{
	pid_t	pid;

	(void)envp;
	pid = fork();
	if (pid < 0)
		perror("fork failed");
	else if (pid == 0)
	{
		if (execvp(cmd[0], cmd) == -1)
		{
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, NULL, 0);
}
