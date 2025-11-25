/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_handlers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:02:01 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/25 13:47:19 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	add_token_for_char(t_tokenizer *tok, char *input, char c,
		int is_double)
{
	char	tmp_buf[3];
	char	tmp_buf[2];

	if (is_double)
	{
		tmp_buf[0] = c;
		tmp_buf[1] = input[tok->i + 1];
		tmp_buf[2] = '\0';
		add_token(&tok->tokens, &tok->count, tmp_buf, tok);
		tok->i += 2;
	}
	else
	{
		tmp_buf[0] = c;
		tmp_buf[1] = '\0';
		add_token(&tok->tokens, &tok->count, tmp_buf, tok);
		tok->i++;
	}
}

void	handle_special_chars(t_tokenizer *tok, char *input)
{
	char	c;

	c = input[tok->i];
	flush_buffer(tok);
	if ((c == '<' && input[tok->i + 1] == '<') || (c == '>' && input[tok->i
			+ 1] == '>') || (c == '|' && input[tok->i + 1] == '|') || (c == '&'
			&& input[tok->i + 1] == '&'))
		add_token_for_char(tok, input, c, 1);
	else
		add_token_for_char(tok, input, c, 0);
}

static void	handle_single_quote(t_tokenizer *tok)
{
	if (!tok->sq && ft_strlen(tok->buf) == 0)
		tok->quote_type = 1;
	if (!tok->sq && ft_strlen(tok->buf) > 0)
	{
		tok->pending_no_space = 1;
		flush_buffer(tok);
	}
	tok->sq = !tok->sq;
	tok->i++;
}

static void	handle_double_quote(t_tokenizer *tok)
{
	if (!tok->dq && ft_strlen(tok->buf) == 0)
		tok->quote_type = 2;
	if (!tok->dq && ft_strlen(tok->buf) > 0)
	{
		tok->pending_no_space = 1;
		flush_buffer(tok);
	}
	tok->dq = !tok->dq;
	tok->i++;
}

void	handle_quotes(t_tokenizer *tok, char c)
{
	if (c == '\'' && tok->dq == 0)
	{
		handle_single_quote(tok);
		return ;
	}
	if (c == '"' && tok->sq == 0)
	{
		handle_double_quote(tok);
		return ;
	}
}
