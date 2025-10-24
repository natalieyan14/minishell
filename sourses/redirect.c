/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:06:09 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 03:23:59 by natalieyan       ###   ########.fr       */
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

static int	setup_output_redirections(t_redir *redir_list)
{
	t_redir	*curr;
	int		fd;
	int		flags;
	int		*fds;
	int		count;
	int		i;

	if (!redir_list)
		return (0);
	count = 0;
	curr = redir_list;
	while (curr && count++, curr = curr->next)
		;
	fds = malloc(sizeof(int) * count);
	if (!fds)
		return (-1);
	i = 0;
	curr = redir_list;
	while (curr)
	{
		flags = O_CREAT | O_WRONLY;
		if (curr->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd = open(curr->filename, flags, 0644);
		if (fd < 0)
		{
			perror(curr->filename);
			while (--i >= 0)
				close(fds[i]);
			free(fds);
			SET_EXIT_STATUS(1);
			return (-1);
		}
		fds[i++] = fd;
		curr = curr->next;
	}
	for (i = 0; i < count - 1; i++)
		close(fds[i]);
	if (dup2(fds[count - 1], STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fds[count - 1]);
		free(fds);
		SET_EXIT_STATUS(1);
		return (-1);
	}
	close(fds[count - 1]);
	free(fds);
	return (0);
}

int	setup_redirections(t_command *cmd)
{
	if (setup_input_redirection(cmd->input) < 0)
		return (-1);
	if (setup_output_redirections(cmd->output_list) < 0)
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
