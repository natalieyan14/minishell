/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 17:42:55 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/24 17:47:47 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	check_signal_status(void)
{
	if (g_signal_received == SIGINT)
		return (130);
	if (g_signal_received == SIGQUIT)
		return (131);
	return (0);
}

void	handle_child_signal_exit(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			write(STDOUT_FILENO, "Quit\n", 5);
		set_exit_status(128 + sig);
	}
	else
		set_exit_status(WEXITSTATUS(status));
	g_signal_received = 0;
}
