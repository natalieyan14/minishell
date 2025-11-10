/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_handlers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 03:50:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/10 18:19:10 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	process_double_char(t_tokenizer *tok, char *input, char c)
{
	char	tmp_buf[3];

	tmp_buf[0] = c;
	tmp_buf[1] = input[tok->i + 1];
	tmp_buf[2] = '\0';
	add_token(&tok->tokens, &tok->count, tmp_buf);
	tok->i += 2;
}

static void	process_single_char(t_tokenizer *tok, char c)
{
	char	tmp_buf[2];

	tmp_buf[0] = c;
	tmp_buf[1] = '\0';
	add_token(&tok->tokens, &tok->count, tmp_buf);
	tok->i++;
}

void	handle_special_chars(t_tokenizer *tok, char *input)
{
	char	c;

	c = input[tok->i];
	flush_buffer(tok);
	if ((c == '<' && input[tok->i + 1] == '<') || (c == '>' && input[tok->i
				+ 1] == '>') || (c == '|'
			&& input[tok->i + 1] == '|') || (c == '&'
			&& input[tok->i + 1] == '&'))
		process_double_char(tok, input, c);
	else
		process_single_char(tok, c);
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

void	flush_buffer(t_tokenizer *tok)
{
	if (ft_strlen(tok->buf) > 0 || tok->quote_type != 0)
	{
		add_token_with_quotes(&tok->tokens, &tok->count, tok->buf,
			tok->quote_type);
		free(tok->buf);
		tok->buf = ft_strdup("");
		tok->quote_type = 0;
	}
}
