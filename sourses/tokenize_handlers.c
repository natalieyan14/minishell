/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_handlers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 03:50:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/26 03:50:34 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	handle_special_chars(t_tokenizer *tok, char *input)
{
	char	tmp_buf[3];
	char	c;

	c = input[tok->i];
	if (ft_strlen(tok->buf) > 0)
	{
		add_token_with_quotes(&tok->tokens, &tok->count, tok->buf,
			tok->quote_type);
		free(tok->buf);
		tok->buf = ft_strdup("");
		tok->quote_type = 0;
	}
	if ((c == '<' && input[tok->i + 1] == '<') || (c == '>' && input[tok->i
			+ 1] == '>'))
	{
		tmp_buf[0] = c;
		tmp_buf[1] = input[tok->i + 1];
		tmp_buf[2] = '\0';
		add_token(&tok->tokens, &tok->count, tmp_buf);
		tok->i += 2;
		return ;
	}
	tmp_buf[0] = c;
	tmp_buf[1] = '\0';
	add_token(&tok->tokens, &tok->count, tmp_buf);
	tok->i++;
}

void	handle_quotes(t_tokenizer *tok, char c)
{
	if (c == '\'' && tok->dq == 0)
	{
		if (!tok->sq)
			tok->quote_type = 1;
		tok->sq = !tok->sq;
		tok->i++;
		return ;
	}
	if (c == '"' && tok->sq == 0)
	{
		if (!tok->dq)
			tok->quote_type = 2;
		tok->dq = !tok->dq;
		tok->i++;
		return ;
	}
}

static int	check_next(t_token *t, int i)
{
	if (t[i].type == T_HEREDOC && t[i + 1].type == T_HEREDOC)
		return (err_message("syntax error near unexpected token `<'\n", "", ""),
			SET_EXIT_STATUS(2), -1);
	if ((t[i].type == T_IN_REDIR && t[i + 1].type != T_IN_FILE)
		|| (t[i].type == T_OUT_REDIR && t[i + 1].type != T_OUT_FILE)
		|| (t[i].type == T_APPEND_REDIR && t[i + 1].type != T_APPEND_FILE)
		|| (t[i].type == T_HEREDOC && t[i + 1].type != T_LIMITER))
		return (err_message("syntax error ", t[i + 1].str, "'\n"),
			SET_EXIT_STATUS(2), -1);
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
				SET_EXIT_STATUS(2), -1);
		if (i + 1 < count && check_next(t, i) < 0)
			return (-1);
		if (t[i].type == T_PIPE && (i == 0 || i + 1 == count || t[i
				+ 1].type == T_PIPE))
			return (err_message("syntax error near unexpected token'\n", "",
					""), SET_EXIT_STATUS(2), -1);
		i++;
	}
	return (1);
}
