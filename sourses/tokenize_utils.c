/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:02:08 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/24 18:09:47 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

void	append_char(char **buf, char c)
{
	int		len;
	char	*tmp;

	len = ft_strlen(*buf);
	tmp = malloc(len + 2);
	if (!tmp)
		exit(1);
	ft_memcpy(tmp, *buf, len);
	tmp[len] = c;
	tmp[len + 1] = '\0';
	free(*buf);
	*buf = tmp;
}

void	add_token_with_quotes(t_token **tokens, int *count, char *str,
	t_tokenizer *tok)
{
	t_token	*tmp;

	tmp = malloc(sizeof(t_token) * (*count + 1));
	if (!tmp)
		exit(1);
	if (*tokens)
	{
		ft_memcpy(tmp, *tokens, sizeof(t_token) * (*count));
		free(*tokens);
	}
	*tokens = tmp;
	(*tokens)[*count].str = ft_strdup(str);
	if (!(*tokens)[*count].str)
		exit(1);
	(*tokens)[*count].type = find_type((*tokens)[*count].str, *tokens, *count);
	(*tokens)[*count].quote_type = tok->quote_type;
	(*tokens)[*count].no_space = tok->pending_no_space;
	(*count)++;
}

void	add_token(t_token **tokens, int *count, char *str, t_tokenizer *tok)
{
	add_token_with_quotes(tokens, count, str, tok);
}

void	init_tokenizer(t_tokenizer *tok)
{
	tok->tokens = NULL;
	tok->buf = ft_strdup("");
	tok->count = 0;
	tok->i = 0;
	tok->sq = 0;
	tok->dq = 0;
	tok->quote_type = 0;
}
