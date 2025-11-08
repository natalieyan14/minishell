/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:12:25 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/08 20:24:04 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

extern int	rl_on_new_line(void);

void	setup_heredoc_signals(void)
{
	struct sigaction	sa_int;

	g_signal_received = 0;
	sa_int.sa_handler = sigint_heredoc;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	check_signal_status(void)
{
	int	status;

	status = 0;
	if (g_signal_received == SIGINT)
		status = 130;
	else if (g_signal_received == SIGQUIT)
		status = 131;
	g_signal_received = 0;
	return (status);
}

int	was_interrupted(void)
{
	return (g_signal_received != 0);
}

void	handle_child_signal_exit(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			set_exit_status(130);
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
			set_exit_status(131);
		}
		else if (WTERMSIG(status) == SIGSEGV)
		{
			write(STDERR_FILENO, "Segmentation fault: 11\n", 23);
			set_exit_status(139);
		}
		else
			set_exit_status(128 + WTERMSIG(status));
	}
	else if (WIFEXITED(status))
		set_exit_status(WEXITSTATUS(status));
}
