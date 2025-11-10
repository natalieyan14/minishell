/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 17:02:29 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/11 01:59:25 by natalieyan       ###   ########.fr       */
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

static int	process_single_heredoc(t_token *tokens, int *i, t_env *env_list)
{
	int	should_expand;
	int	original_stdin;

	should_expand = (tokens[*i + 1].quote_type == 0);
	original_stdin = handle_heredoc(tokens[*i + 1].str, should_expand,
			env_list);
	if (original_stdin < 0)
	{
		set_exit_status(1);
		return (-1);
	}
	*i += 2;
	return (original_stdin);
}

int	process_heredocs(t_token *tokens, int count, t_env *env_list)
{
	int	i;
	int	original_stdin;

	i = 0;
	original_stdin = -1;
	while (i < count - 1)
	{
		if (tokens[i].type == T_HEREDOC && tokens[i + 1].type == T_LIMITER)
		{
			original_stdin = process_single_heredoc(tokens, &i, env_list);
			if (original_stdin < 0)
				return (-1);
		}
		else
			i++;
	}
	if (original_stdin >= 0)
		set_exit_status(0);
	return (original_stdin);
}
