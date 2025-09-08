/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 18:40:24 by natalieyan        #+#    #+#             */
/*   Updated: 2025/09/06 13:24:51 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

extern int			g_exit_status;

static t_toktype	find_type(char *str, t_token *tokens, int i)
{
	if (!ft_strcmp(str, "|"))
		return (T_PIPE);
	if (!ft_strcmp(str, "<"))
		return (T_IN_REDIR);
	if (!ft_strcmp(str, ">"))
		return (T_OUT_REDIR);
	if (!ft_strcmp(str, ">>"))
		return (T_APPEND_REDIR);
	if (!ft_strcmp(str, "<<"))
		return (T_HEREDOC);
	if (i > 0)
	{
		if (tokens[i - 1].type == T_IN_REDIR)
			return (T_IN_FILE);
		if (tokens[i - 1].type == T_OUT_REDIR)
			return (T_OUT_FILE);
		if (tokens[i - 1].type == T_APPEND_REDIR)
			return (T_APPEND_FILE);
		if (tokens[i - 1].type == T_HEREDOC)
			return (T_LIMITER);
	}
	return (T_WORD);
}

t_token	*tokenisation(char **args, int count)
{
	t_token	*tokens;
	int		i;

	tokens = malloc(count * sizeof(t_token));
	if (!tokens)
		return (NULL);
	i = 0;
	while (i < count)
	{
		tokens[i].str = ft_strdup(args[i]);
		if (!tokens[i].str)
		{
			while (i-- > 0)
				free(tokens[i].str);
			free(tokens);
			return (NULL);
		}
		tokens[i].type = find_type(args[i], tokens, i);
		i++;
	}
	return (tokens);
}

static int	check_next(t_token *t, int i)
{
	if (t[i].type == T_HEREDOC && t[i + 1].type == T_HEREDOC)
		return (err_message("syntax error near unexpected token `<'\n", "", ""),
			g_exit_status = 2, -1);
	if ((t[i].type == T_IN_REDIR && t[i + 1].type != T_IN_FILE)
		|| (t[i].type == T_OUT_REDIR && t[i + 1].type != T_OUT_FILE)
		|| (t[i].type == T_APPEND_REDIR && t[i + 1].type != T_APPEND_FILE)
		|| (t[i].type == T_HEREDOC && t[i + 1].type != T_LIMITER))
		return (err_message("syntax error ", t[i + 1].str, "'\n"),
			g_exit_status = 2, -1);
	return (1);
}

int	if_invalid_input(t_token *t, int count, int i)
{
	while (++i < count)
	{
		if (i + 1 == count && (t[i].type == T_IN_REDIR
				|| t[i].type == T_OUT_REDIR || t[i].type == T_APPEND_REDIR
				|| t[i].type == T_HEREDOC))
			return (err_message("syntax error near `newline'\n", "", ""),
				g_exit_status = 2, -1);
		if (check_next(t, i) < 0)
			return (-1);
		if (t[i].type == T_PIPE && (i == 0 || i + 1 == count || t[i
				+ 1].type == T_PIPE))
			return (err_message("syntax error near unexpected token `|'\n", "",
					""), g_exit_status = 2, -1);
	}
	return (1);
}