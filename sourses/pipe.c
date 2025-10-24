/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:06:02 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 02:57:33 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	exec_single_command(t_command *cmd, t_env **env_list)
{
	char	**env_array;
	int		status;
	pid_t	pid;
				struct stat st;

	if (!cmd->argc || !cmd->argc[0] || ft_strlen(cmd->argc[0]) == 0)
	{
		SET_EXIT_STATUS(0);
		return (0);
	}
	if (is_builtin(cmd))
	{
		exec_builtin(cmd, env_list);
		return (GET_EXIT_STATUS());
	}
	else
	{
		env_array = list_to_array(*env_list);
		if (!env_array)
			return (1);
		pid = fork();
		if (pid < 0)
		{
			perror("fork failed");
			free(env_array);
			return (1);
		}
		else if (pid == 0)
		{
			if (setup_redirections(cmd) < 0)
				exit(1);
			if (ft_strchr(cmd->argc[0], '/'))
			{
				if (stat(cmd->argc[0], &st) == 0)
				{
					if (S_ISDIR(st.st_mode))
					{
						fprintf(stderr, "%s: is a directory\n", cmd->argc[0]);
						exit(126);
					}
					else if (access(cmd->argc[0], X_OK) != 0)
					{
						perror(cmd->argc[0]);
						exit(126);
					}
				}
				else
				{
					perror(cmd->argc[0]);
					exit(127);
				}
			}
			if (execvp(cmd->argc[0], cmd->argc) == -1)
			{
				if (errno == ENOENT)
				{
					fprintf(stderr, "%s: command not found\n", cmd->argc[0]);
					exit(127);
				}
				else if (errno == EACCES)
				{
					perror(cmd->argc[0]);
					exit(126);
				}
				else
				{
					perror(cmd->argc[0]);
					exit(127);
				}
			}
		}
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				SET_EXIT_STATUS(WEXITSTATUS(status));
			else if (WIFSIGNALED(status))
				SET_EXIT_STATUS(128 + WTERMSIG(status));
		}
		free(env_array);
	}
	return (GET_EXIT_STATUS());
}

static int	count_commands(t_command *cmd_list)
{
	int	count;

	count = 0;
	while (cmd_list)
	{
		count++;
		cmd_list = cmd_list->next;
	}
	return (count);
}

int	execute_pipeline(t_command *cmd_list, t_env **env_list)
{
	int			cmd_count;
	int			**pipes;
	pid_t		*pids;
	int			i;
	t_command	*current;
	int			status;
					struct stat st;

	if (!cmd_list)
		return (0);
	cmd_count = count_commands(cmd_list);
	if (cmd_count == 1)
		return (exec_single_command(cmd_list, env_list));
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pipes || !pids)
	{
		free(pipes);
		free(pids);
		return (1);
	}
	for (i = 0; i < cmd_count - 1; i++)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) < 0)
		{
			perror("pipe failed");
			while (--i >= 0)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
				free(pipes[i]);
			}
			free(pipes);
			free(pids);
			return (1);
		}
	}
	current = cmd_list;
	for (i = 0; i < cmd_count; i++)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("fork failed");
			for (int j = 0; j < cmd_count - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				free(pipes[j]);
			}
			free(pipes);
			free(pids);
			return (1);
		}
		else if (pids[i] == 0)
		{
			if (i > 0)
			{
				if (dup2(pipes[i - 1][0], STDIN_FILENO) < 0)
				{
					perror("dup2 failed");
					exit(1);
				}
			}
			if (i < cmd_count - 1)
			{
				if (dup2(pipes[i][1], STDOUT_FILENO) < 0)
				{
					perror("dup2 failed");
					exit(1);
				}
			}
			for (int j = 0; j < cmd_count - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}
			if (setup_redirections(current) < 0)
				exit(1);
			if (!current->argc || !current->argc[0]
				|| ft_strlen(current->argc[0]) == 0)
				exit(0);
			if (is_builtin(current))
			{
				exec_builtin(current, env_list);
				exit(GET_EXIT_STATUS());
			}
			else
			{
				if (ft_strchr(current->argc[0], '/'))
				{
					if (stat(current->argc[0], &st) == 0)
					{
						if (S_ISDIR(st.st_mode))
						{
							fprintf(stderr, "%s: is a directory\n",
								current->argc[0]);
							exit(126);
						}
						else if (access(current->argc[0], X_OK) != 0)
						{
							perror(current->argc[0]);
							exit(126);
						}
					}
					else
					{
						perror(current->argc[0]);
						exit(127);
					}
				}
				if (execvp(current->argc[0], current->argc) == -1)
				{
					if (errno == ENOENT)
					{
						fprintf(stderr, "%s: command not found\n",
							current->argc[0]);
						exit(127);
					}
					else if (errno == EACCES)
					{
						perror(current->argc[0]);
						exit(126);
					}
					else
					{
						perror(current->argc[0]);
						exit(127);
					}
				}
			}
		}
		current = current->next;
	}
	for (i = 0; i < cmd_count - 1; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
	}
	free(pipes);
	for (i = 0; i < cmd_count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				SET_EXIT_STATUS(WEXITSTATUS(status));
			else if (WIFSIGNALED(status))
				SET_EXIT_STATUS(128 + WTERMSIG(status));
		}
	}
	free(pids);
	return (GET_EXIT_STATUS());
}
