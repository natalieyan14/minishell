/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 04:00:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/29 21:22:44 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	check_next(t_token *t, int i)
{
	if (t[i].type == T_HEREDOC && t[i + 1].type == T_HEREDOC)
		return (err_message("syntax error near unexpected token `<'\n", "", ""),
			set_exit_status(2), -1);
	if ((t[i].type == T_IN_REDIR && t[i + 1].type != T_IN_FILE)
		|| (t[i].type == T_OUT_REDIR && t[i + 1].type != T_OUT_FILE)
		|| (t[i].type == T_APPEND_REDIR && t[i + 1].type != T_APPEND_FILE)
		|| (t[i].type == T_HEREDOC && t[i + 1].type != T_LIMITER))
		return (err_message("syntax error ", t[i + 1].str, "'\n"),
			set_exit_status(2), -1);
	return (1);
}

int	if_invalid_input(t_token *t, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (i + 1 == count && (t[i].type == T_IN_REDIR
				|| t[i].type == T_OUT_REDIR || t[i].type == T_APPEND_REDIR
				|| t[i].type == T_HEREDOC))
			return (err_message("syntax error near `newline'\n", "", ""),
				set_exit_status(2), -1);
		if (i + 1 < count && check_next(t, i) < 0)
			return (-1);
		if (t[i].type == T_PIPE && (i == 0 || i + 1 == count || t[i
					+ 1].type == T_PIPE))
			return (err_message("syntax error near unexpected token `|'\n", "",
					""), set_exit_status(2), -1);
		i++;
	}
	return (1);
}
