/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:01:49 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/24 18:30:36 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	g_signal_received = 0;

static void	sigint_interactive(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_heredoc(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
}

void	setup_interactive_signals(void)
{
	struct sigaction	sa_int;

	g_signal_received = 0;
	memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = sigint_interactive;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_heredoc_signals(void)
{
	g_signal_received = 0;
	signal(SIGINT, sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_execution_signals(void)
{
	g_signal_received = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
