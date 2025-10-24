/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:25:51 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 02:48:23 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

volatile sig_atomic_t	g_signal_received = 0;

static int	get_exit_status(int set_value, int new_value)
{
	static int	exit_status = 0;

	if (set_value)
		exit_status = new_value;
	return (exit_status);
}

static void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

static void	handle_sigquit(int sig)
{
	(void)sig;
	return ;
}

void	setup_interactive_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = handle_sigquit;
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_execution_signals(void)
{
	struct sigaction	sa_int;

	if (g_signal_received == 1)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else
	{
		sigemptyset(&sa_int.sa_mask);
		sa_int.sa_handler = SIG_DFL;
		sa_int.sa_flags = 0;
		sigaction(SIGINT, &sa_int, NULL);
		signal(SIGQUIT, SIG_IGN);
	}
}

void	set_exit_status(int status)
{
	get_exit_status(1, status);
}

int	get_current_exit_status(void)
{
	return (get_exit_status(0, 0));
}

int	check_signal_status(void)
{
	if (g_signal_received == SIGINT)
	{
		set_exit_status(130);
		g_signal_received = 0;
		return (130);
	}
	return (0);
}
