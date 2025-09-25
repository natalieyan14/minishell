/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:50:28 by natalieyan        #+#    #+#             */
/*   Updated: 2025/09/25 14:17:59 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	exec_command(char *cmd, char **envp)
{
	pid_t	pid;
	char	*argv[] = {cmd, NULL};
	char	**env;

	if (ft_strcmp(cmd, "env") == 0)
	{
		env = env_to_array(init_env(envp));
		print_env(env);
		return ;
	}
	pid = fork();
	if (pid < 0)
		perror("fork failed");
	else if (pid == 0)
	{
		if (execve(cmd, argv, envp) == -1)
			perror("execve failed");
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, NULL, 0);
}
