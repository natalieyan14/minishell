/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:59:50 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 16:59:53 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	handle_eof_error(char *full_line)
{
	write(STDERR_FILENO,
		"minishell: unexpected EOF while looking for matching quote\n", 59);
	set_exit_status(2);
	free(full_line);
	return (0);
}

char	*append_continuation(char *full_line, char *continuation)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(full_line, "\n");
	free(full_line);
	result = ft_strjoin(tmp, continuation);
	free(tmp);
	free(continuation);
	return (result);
}

char	*read_continuation_line(char *full_line, int is_interactive)
{
	char	*continuation;

	if (!is_interactive)
	{
		handle_eof_error(full_line);
		return (NULL);
	}
	continuation = readline("> ");
	if (!continuation)
	{
		handle_eof_error(full_line);
		return (NULL);
	}
	return (append_continuation(full_line, continuation));
}
