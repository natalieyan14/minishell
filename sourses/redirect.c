/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:06:09 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 02:57:33 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	setup_input_redirection(char *filename)
{
	int	fd;

	if (!filename)
		return (0);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror(filename);
		SET_EXIT_STATUS(1);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		SET_EXIT_STATUS(1);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	setup_output_redirection(char *filename, int append)
{
	int	fd;
	int	flags;

	if (!filename)
		return (0);
	flags = O_CREAT | O_WRONLY;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		perror(filename);
		SET_EXIT_STATUS(1);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		SET_EXIT_STATUS(1);
		return (-1);
	}
	close(fd);
	return (0);
}

int	setup_redirections(t_command *cmd)
{
	if (setup_input_redirection(cmd->input) < 0)
		return (-1);
	if (setup_output_redirection(cmd->output, cmd->append) < 0)
		return (-1);
	return (0);
}

void	exec_command_with_redirections(t_command *cmd, char **envp)
{
	pid_t	pid;
	int		status;

	(void)envp;
	if (!cmd || !cmd->argc || !cmd->argc[0])
		return ;
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		SET_EXIT_STATUS(1);
		return ;
	}
	else if (pid == 0)
	{
		if (setup_redirections(cmd) < 0)
			exit(1);
		if (execvp(cmd->argc[0], cmd->argc) == -1)
		{
			perror("execve failed");
			exit(127);
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
}
