/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 20:05:42 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/09 20:17:14 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	**create_pipes(int pipe_count)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * pipe_count);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < pipe_count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) < 0)
		{
			while (--i >= 0)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
				free(pipes[i]);
			}
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	cleanup_pipes(int **pipes, int pipe_count)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < pipe_count)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

int	wait_and_cleanup(int **pipes, pid_t *pids, int cmd_count)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
			handle_child_signal_exit(status);
		i++;
	}
	cleanup_pipes(pipes, cmd_count - 1);
	free(pids);
	setup_interactive_signals();
	return (get_current_exit_status());
}

int	cleanup_and_error(int **pipes, pid_t *pids, int pipe_count)
{
	if (pipes)
		cleanup_pipes(pipes, pipe_count);
	if (pids)
		free(pids);
	return (1);
}

int	fork_error(int **pipes, pid_t *pids, int pipe_count)
{
	perror("fork failed");
	cleanup_pipes(pipes, pipe_count);
	free(pids);
	setup_interactive_signals();
	return (1);
}
