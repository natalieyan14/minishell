/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:06:31 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/26 03:52:38 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_toktype	find_type(char *str, t_token *tokens, int i)
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

static void	handle_whitespace(t_tokenizer *tok)
{
	if (ft_strlen(tok->buf) > 0)
	{
		add_token_with_quotes(&tok->tokens, &tok->count, tok->buf,
			tok->quote_type);
		free(tok->buf);
		tok->buf = ft_strdup("");
		tok->quote_type = 0;
	}
	tok->i++;
}

static void	process_character(t_tokenizer *tok, char *input)
{
	char	c;

	c = input[tok->i];
	if ((c == '\'' && tok->dq == 0) || (c == '"' && tok->sq == 0))
		handle_quotes(tok, c);
	else if (isspace((unsigned char)c) && tok->sq == 0 && tok->dq == 0)
		handle_whitespace(tok);
	else if (is_special_char(c) && tok->sq == 0 && tok->dq == 0)
		handle_special_chars(tok, input);
	else
	{
		append_char(&tok->buf, c);
		tok->i++;
	}
}

t_token	*tokenisation(char *input, int *out_count)
{
	t_tokenizer	tok;

	init_tokenizer(&tok);
	while (input[tok.i])
		process_character(&tok, input);
	if (ft_strlen(tok.buf) > 0)
		add_token_with_quotes(&tok.tokens, &tok.count, tok.buf, tok.quote_type);
	free(tok.buf);
	*out_count = tok.count;
	return (tok.tokens);
}
