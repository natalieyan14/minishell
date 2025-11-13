/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:00:41 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 17:00:44 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	setup_pipeline_resources(int cmd_count, int ***pipes, pid_t **pids)
{
	*pipes = create_pipes(cmd_count - 1);
	*pids = malloc(sizeof(pid_t) * cmd_count);
	if (!*pipes || !*pids)
		return (cleanup_and_error(*pipes, *pids, cmd_count - 1));
	return (0);
}

static int	execute_pipeline_children(t_command *cmd_list, t_env **env_list,
		int **pipes, pid_t *pids)
{
	int			cmd_count;
	int			i;
	t_command	*current;
	t_pipe_ctx	ctx;

	cmd_count = count_commands(cmd_list);
	ctx.pipes = pipes;
	ctx.cmd_count = cmd_count;
	ctx.env_list = env_list;
	current = cmd_list;
	i = 0;
	while (current && i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			exec_child(current, &ctx, i);
		else if (pids[i] < 0)
			return (fork_error(pipes, pids, cmd_count - 1));
		current = current->next;
		i++;
	}
	return (cmd_count);
}

static void	update_last_cmd_underscore(t_command *cmd_list, t_env **env_list)
{
	t_command	*last_cmd;
	char		*executable;

	last_cmd = cmd_list;
	while (last_cmd && last_cmd->next)
		last_cmd = last_cmd->next;
	if (last_cmd && last_cmd->argc && last_cmd->argc[0])
	{
		if (is_builtin(last_cmd))
		{
			update_underscore_var(env_list, last_cmd->argc[0]);
		}
		else
		{
			executable = find_executable_in_path(last_cmd->argc[0], *env_list);
			if (executable)
			{
				update_underscore_var(env_list, executable);
				free(executable);
			}
		}
	}
}

int	execute_pipeline(t_command *cmd_list, t_env **env_list)
{
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	int		exit_status;

	if (!cmd_list)
		return (0);
	cmd_count = count_commands(cmd_list);
	if (cmd_count == 1)
		return (exec_single_command(cmd_list, env_list));
	if (setup_pipeline_resources(cmd_count, &pipes, &pids) != 0)
		return (1);
	setup_execution_signals();
	cmd_count = execute_pipeline_children(cmd_list, env_list, pipes, pids);
	if (cmd_count <= 0)
		return (1);
	close_all_pipes(pipes, cmd_count);
	exit_status = wait_and_cleanup(pipes, pids, cmd_count);
	update_last_cmd_underscore(cmd_list, env_list);
	return (exit_status);
}
