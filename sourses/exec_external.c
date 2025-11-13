/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 20:30:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/13 14:34:08 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	handle_exec_not_found(char **envp, char *cmd_name)
{
	free_string_array(envp);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	if (ft_strchr(cmd_name, '/'))
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
}

static int	handle_exec_access_error(char *executable, char *cmd_name)
{
	struct stat	file_stat;

	if (stat(executable, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	}
	return (126);
}

static void	handle_exec_error(char **envp, char *executable, char *cmd_name)
{
	int	exit_code;

	exit_code = 127;
	if (errno == EACCES)
		exit_code = handle_exec_access_error(executable, cmd_name);
	else if (errno == ENOENT)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		exit_code = 127;
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
	}
	free_string_array(envp);
	free(executable);
	exit(exit_code);
}

void	exec_external(t_command *cmd, t_env **env_list)
{
	char	*executable;
	char	**envp;

	if (!cmd || !cmd->argc || !cmd->argc[0])
		exit(127);
	envp = env_to_array(*env_list);
	if (cmd->argc[0] && (ft_strcmp(cmd->argc[0], ".") == 0 
	|| ft_strcmp(cmd->argc[0], "..") == 0))
	{
		handle_exec_not_found(envp, cmd->argc[0]);
		exit(127);
	}
	executable = find_executable_in_path(cmd->argc[0], *env_list);
	if (!executable)
	{
		handle_exec_not_found(envp, cmd->argc[0]);
		exit(127);
	}
	if (execve(executable, cmd->argc, envp) == -1)
		handle_exec_error(envp, executable, cmd->argc[0]);
}

// #include "header.h"

// static void	handle_exec_not_found(char **envp, char *cmd_name)
// {
// 	free_string_array(envp);
// 	ft_putstr_fd("minishell: ", STDERR_FILENO);
// 	ft_putstr_fd(cmd_name, STDERR_FILENO);
// 	if (ft_strchr(cmd_name, '/'))
// 		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
// 	else
// 		ft_putstr_fd(": command not found\n", STDERR_FILENO);
// 	exit(127);
// }

// static int	handle_exec_access_error(char *executable, char *cmd_name)
// {
// 	struct stat	file_stat;

// 	if (stat(executable, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
// 	{
// 		ft_putstr_fd("minishell: ", STDERR_FILENO);
// 		ft_putstr_fd(cmd_name, STDERR_FILENO);
// 		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
// 	}
// 	else
// 	{
// 		ft_putstr_fd("minishell: ", STDERR_FILENO);
// 		ft_putstr_fd(cmd_name, STDERR_FILENO);
// 		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
// 	}
// 	return (126);
// }

// static void	handle_exec_error(char **envp, char *executable, char *cmd_name)
// {
// 	int	exit_code;

// 	exit_code = 127;
// 	if (errno == EACCES)
// 		exit_code = handle_exec_access_error(executable, cmd_name);
// 	else if (errno == ENOENT)
// 	{
// 		ft_putstr_fd("minishell: ", STDERR_FILENO);
// 		ft_putstr_fd(cmd_name, STDERR_FILENO);
// 		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
// 		exit_code = 127;
// 	}
// 	else
// 	{
// 		ft_putstr_fd("minishell: ", STDERR_FILENO);
// 		ft_putstr_fd(cmd_name, STDERR_FILENO);
// 		ft_putstr_fd(": ", STDERR_FILENO);
// 		perror("");
// 	}
// 	free_string_array(envp);
// 	free(executable);
// 	exit(exit_code);
// }

// void	exec_external(t_command *cmd, t_env **env_list)
// {
// 	char	*executable;
// 	char	**envp;

// 	if (!cmd || !cmd->argc || !cmd->argc[0])
// 		exit(127);
// 	envp = env_to_array(*env_list);
// 	executable = find_executable_in_path(cmd->argc[0], *env_list);
// 	if (!executable)
// 		handle_exec_not_found(envp, cmd->argc[0]);
// 	if (execve(executable, cmd->argc, envp) == -1)
// 		handle_exec_error(envp, executable, cmd->argc[0]);
// }
