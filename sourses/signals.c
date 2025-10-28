/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 04:10:44 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/28 22:44:57 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

/* External readline functions */
extern int				rl_on_new_line(void);

volatile sig_atomic_t	g_signal_received = 0;

static void	sigint_interactive(int sig)
{
	(void)sig;
	set_exit_status(130);
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

static void	sigint_heredoc(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	set_exit_status(130);
	close(STDIN_FILENO);
}

void	setup_interactive_signals(void)
{
	struct sigaction	sa_int;

	g_signal_received = 0;
	sa_int.sa_handler = sigint_interactive;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_execution_signals(void)
{
	g_signal_received = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

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
		else
			set_exit_status(128 + WTERMSIG(status));
	}
	else if (WIFEXITED(status))
		set_exit_status(WEXITSTATUS(status));
}