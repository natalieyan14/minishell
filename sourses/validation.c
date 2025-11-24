/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:02:48 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/24 18:19:49 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	handle_syntax_error_newline(void)
{
	err_message("minishell: syntax error near `newline'", "", "");
	set_exit_status(2);
	return (-1);
}

static int	handle_syntax_error_pipe(void)
{
	err_message("minishell: syntax error near unexpected token `|'", "", "");
	set_exit_status(2);
	return (-1);
}

static int	handle_syntax_error_unexpected(t_toktype type)
{
	if (type == T_AND)
		err_message("minishell: syntax error near unexpected token `&&'", "",
			"");
	else if (type == T_OR)
		err_message("minishell: syntax error near unexpected token `||'", "",
			"");
	else
		err_message("minishell: syntax error near unexpected token", "", "");
	set_exit_status(2);
	return (-1);
}

static int	check_next(t_token *t, int i)
{
	if (t[i].type == T_HEREDOC && t[i + 1].type == T_HEREDOC)
	{
		err_message("minishell: syntax error near unexpected token `<<'", "",
			"");
		set_exit_status(2);
		return (-1);
	}
	if ((t[i].type == T_IN_REDIR && t[i + 1].type != T_IN_FILE)
		|| (t[i].type == T_OUT_REDIR && t[i + 1].type != T_OUT_FILE)
		|| (t[i].type == T_APPEND_REDIR && t[i + 1].type != T_APPEND_FILE)
		|| (t[i].type == T_HEREDOC && t[i + 1].type != T_LIMITER))
	{
		err_message("minishell: syntax error near unexpected token `", t[i
			+ 1].str, "");
		set_exit_status(2);
		return (-1);
	}
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
			return (handle_syntax_error_newline());
		if (i + 1 < count && check_next(t, i) < 0)
			return (-1);
		if (t[i].type == T_PIPE && (i == 0 || i + 1 == count || t[i
					+ 1].type == T_PIPE))
			return (handle_syntax_error_pipe());
		if ((t[i].type == T_OR || t[i].type == T_AND) && (i == 0 || i
				+ 1 == count))
			return (handle_syntax_error_unexpected(t[i].type));
		i++;
	}
	return (1);
}
