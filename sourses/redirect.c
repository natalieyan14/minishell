/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:06:09 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/29 00:52:55 by natalieyan       ###   ########.fr       */
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
		set_exit_status(1);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		set_exit_status(1);
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
			set_exit_status(1);
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
		set_exit_status(1);
		return (-1);
	}
	close(fds[count - 1]);
	free(fds);
	return (0);
}

int	validate_all_input_redirections(t_input_redir *input_list)
{
	t_input_redir	*curr;
	int				fd;

	if (!input_list)
		return (0);
	curr = input_list;
	while (curr)
	{
		fd = open(curr->filename, O_RDONLY);
		if (fd < 0)
		{
			perror(curr->filename);
			set_exit_status(1);
			return (-1);
		}
		close(fd);
		curr = curr->next;
	}
	return (0);
}

int	setup_redirections(t_command *cmd)
{
	int	input_result;
	int	output_result;
	int	validation_result;

	/* Validate all input redirections first - this handles most cases correctly */
	validation_result = validate_all_input_redirections(cmd->input_list);
	if (validation_result < 0)
		return (-1);
	
	/* Then process output redirections */
	output_result = setup_output_redirections(cmd->output_list);
	if (output_result < 0)
		return (-1);
	
	/* Finally set up the actual input redirection */
	input_result = setup_input_redirection(cmd->input);
	if (input_result < 0)
		return (-1);
	return (0);
}

void	exec_command_with_redirections(t_command *cmd, char **envp)
{
	pid_t	pid;
	int		status;
	char	*executable;
	if (!cmd || !cmd->argc || !cmd->argc[0])
		return ;
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		set_exit_status(1);
		return ;
	}
	else if (pid == 0)
	{
		if (setup_redirections(cmd) < 0)
			exit(1);
		executable = find_executable_in_path(cmd->argc[0]);
		if (!executable)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->argc[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			exit(127);
		}
		if (execve(executable, cmd->argc, envp) == -1)
		{
			perror("execve failed");
			exit(127);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			set_exit_status(WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			set_exit_status(128 + WTERMSIG(status));
	}
}

int	setup_ordered_redirections(t_command *cmd)
{
	t_ordered_redir	*curr;
	int				fd;
	int				flags;

	if (!cmd->ordered_redirs)
	{
		/* Fallback to old method if no ordered redirections */
		return (setup_redirections(cmd));
	}
	
	/* First pass: validate inputs and handle redirections in order */
	curr = cmd->ordered_redirs;
	while (curr)
	{
		if (curr->type == REDIR_INPUT)
		{
			fd = open(curr->filename, O_RDONLY);
			if (fd < 0)
			{
				perror(curr->filename);
				set_exit_status(1);
				return (-1);
			}
			close(fd);
		}
		else if (curr->type == REDIR_OUTPUT || curr->type == REDIR_APPEND)
		{
			/* Create output files as we encounter them */
			flags = O_CREAT | O_WRONLY;
			if (curr->type == REDIR_APPEND)
				flags |= O_APPEND;
			else
				flags |= O_TRUNC;
			fd = open(curr->filename, flags, 0644);
			if (fd < 0)
			{
				perror(curr->filename);
				set_exit_status(1);
				return (-1);
			}
			if (dup2(fd, STDOUT_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				set_exit_status(1);
				return (-1);
			}
			close(fd);
		}
		curr = curr->next;
	}
	
	/* Second pass: set up input redirections */
	curr = cmd->ordered_redirs;
	while (curr)
	{
		if (curr->type == REDIR_INPUT)
		{
			fd = open(curr->filename, O_RDONLY);
			if (dup2(fd, STDIN_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				set_exit_status(1);
				return (-1);
			}
			close(fd);
		}
		curr = curr->next;
	}
	return (0);
}
