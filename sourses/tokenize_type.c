/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 23:20:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/22 23:03:23 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static t_toktype	check_special_operators(char *str)
{
	if (!ft_strcmp(str, "|"))
		return (T_PIPE);
	if (!ft_strcmp(str, "||"))
		return (T_OR);
	if (!ft_strcmp(str, "&&"))
		return (T_AND);
	if (!ft_strcmp(str, "<"))
		return (T_IN_REDIR);
	if (!ft_strcmp(str, ">"))
		return (T_OUT_REDIR);
	if (!ft_strcmp(str, ">>"))
		return (T_APPEND_REDIR);
	if (!ft_strcmp(str, "<<"))
		return (T_HEREDOC);
	return (T_WORD);
}

static t_toktype	check_file_type(t_token *tokens, int i)
{
	if (tokens[i - 1].type == T_IN_REDIR)
		return (T_IN_FILE);
	if (tokens[i - 1].type == T_OUT_REDIR)
		return (T_OUT_FILE);
	if (tokens[i - 1].type == T_APPEND_REDIR)
		return (T_APPEND_FILE);
	if (tokens[i - 1].type == T_HEREDOC)
		return (T_LIMITER);
	return (T_WORD);
}

t_toktype	find_type(char *str, t_token *tokens, int i)
{
	t_toktype	type;

	type = check_special_operators(str);
	if (type != T_WORD)
		return (type);
	if (i > 0)
		return (check_file_type(tokens, i));
	return (T_WORD);
}
