/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 20:05:42 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/09 21:10:59 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	count_commands(t_command *cmd_list)
{
	int			count;
	t_command	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static void	setup_pipe_io(int **pipes, int i, int cmd_count)
{
	if (i > 0 && dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
	{
		perror("dup2 input failed");
		exit(1);
	}
	if (i < cmd_count - 1 && dup2(pipes[i][1], STDOUT_FILENO) == -1)
	{
		perror("dup2 output failed");
		exit(1);
	}
}

void	close_all_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	exec_child(t_command *cmd, t_pipe_ctx *ctx, int i)
{
	setup_child_signals();
	setup_pipe_io(ctx->pipes, i, ctx->cmd_count);
	close_all_pipes(ctx->pipes, ctx->cmd_count);
	if (setup_ordered_redirections(cmd) < 0)
		exit(1);
	if (!cmd->argc || !cmd->argc[0] || ft_strlen(cmd->argc[0]) == 0)
		exit(0);
	if (is_builtin(cmd))
	{
		exec_builtin(cmd, ctx->env_list);
		exit(get_current_exit_status());
	}
	exec_external(cmd, ctx->env_list);
}
