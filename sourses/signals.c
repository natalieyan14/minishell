/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 04:10:44 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/29 21:13:59 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

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

void	sigint_heredoc(int sig)
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
