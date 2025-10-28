/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:06:02 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/29 00:52:55 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	exec_single_command(t_command *cmd, t_env **env_list)
{
	char		**env_array;
	int			status;
	pid_t		pid;
	struct stat	st;
	int			saved_stdin;
	int			saved_stdout;
	char		*executable;

	if (!cmd->argc || !cmd->argc[0] || ft_strlen(cmd->argc[0]) == 0)
	{
		set_exit_status(0);
		return (0);
	}
	if (is_builtin(cmd))
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (setup_ordered_redirections(cmd) < 0)
		{
			close(saved_stdin);
			close(saved_stdout);
			return (1);
		}
		exec_builtin(cmd, env_list);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (get_current_exit_status());
	}
	else
	{
		env_array = list_to_array(*env_list);
		if (!env_array)
			return (1);
		setup_execution_signals();
		pid = fork();
		if (pid < 0)
		{
			perror("fork failed");
			free_string_array(env_array);
			setup_interactive_signals();
			return (1);
		}
		else if (pid == 0)
		{
			setup_child_signals();
			if (setup_ordered_redirections(cmd) < 0)
				exit(1);
			if (ft_strchr(cmd->argc[0], '/'))
			{
				if (stat(cmd->argc[0], &st) == 0)
				{
					if (S_ISDIR(st.st_mode))
					{
						ft_putstr_fd(cmd->argc[0], STDERR_FILENO);
						ft_putstr_fd(": is a directory\n", STDERR_FILENO);
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
			executable = find_executable_in_path(cmd->argc[0]);
			if (!executable)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(cmd->argc[0], STDERR_FILENO);
				ft_putstr_fd(": command not found\n", STDERR_FILENO);
				exit(127);
			}
			if (execve(executable, cmd->argc, env_array) == -1)
			{
				perror("execve failed");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			waitpid(pid, &status, 0);
			handle_child_signal_exit(status);
			setup_interactive_signals();
		}
		free_string_array(env_array);
	}
	return (get_current_exit_status());
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
	int cmd_count;
	int **pipes;
	pid_t *pids;
	int i;
	t_command *current;
	int status;
	struct stat st;
	char *executable;
	char **envp;

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
	setup_execution_signals();
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
			setup_interactive_signals();
			return (1);
		}
		else if (pids[i] == 0)
		{
			setup_child_signals();
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
			if (setup_ordered_redirections(current) < 0)
				exit(1);
			if (!current->argc || !current->argc[0]
				|| ft_strlen(current->argc[0]) == 0)
				exit(0);
			if (is_builtin(current))
			{
				exec_builtin(current, env_list);
				exit(get_current_exit_status());
			}
			else
			{
				if (ft_strchr(current->argc[0], '/'))
				{
					if (stat(current->argc[0], &st) == 0)
					{
						if (S_ISDIR(st.st_mode))
						{
							ft_putstr_fd(current->argc[0], STDERR_FILENO);
							ft_putstr_fd(": is a directory\n", STDERR_FILENO);
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
				executable = find_executable_in_path(current->argc[0]);
				if (!executable)
				{
					ft_putstr_fd("minishell: ", STDERR_FILENO);
					ft_putstr_fd(current->argc[0], STDERR_FILENO);
					ft_putstr_fd(": command not found\n", STDERR_FILENO);
					exit(127);
				}
				envp = list_to_array(*env_list);
				if (execve(executable, current->argc, envp) == -1)
				{
					perror("execve failed");
					exit(EXIT_FAILURE);
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
			handle_child_signal_exit(status);
	}
	free(pids);
	setup_interactive_signals();
	return (get_current_exit_status());
}