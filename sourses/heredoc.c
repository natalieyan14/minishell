/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:22:51 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/13 13:38:39 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:19:54 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/12 21:42:08 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	heredoc_child(int pipe_fd[2], char *limiter, int should_expand,
		t_env *env_list)
{
	close(pipe_fd[0]);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	run_heredoc(pipe_fd[1], limiter, should_expand, env_list);
	close(pipe_fd[1]);
	exit(0);
}

int	handle_parent_process(pid_t pid, int pipe_fd[2])
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		close(pipe_fd[0]);
		set_exit_status(128 + WTERMSIG(status));
		return (-1);
	}
	if (WEXITSTATUS(status) != 0)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (pipe_fd[0]);
}

int	handle_fork_error(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	perror("minishell: fork");
	return (-1);
}

int	handle_heredoc(char *limiter, int should_expand, t_env *env_list)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (!limiter)
		return (-1);
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(pipe_fd));
	if (pid == 0)
		heredoc_child(pipe_fd, limiter, should_expand, env_list);
	return (handle_parent_process(pid, pipe_fd));
}
