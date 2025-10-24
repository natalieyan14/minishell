/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:06:31 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 02:48:23 by natalieyan       ###   ########.fr       */
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

static int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static void	append_char(char **buf, char c)
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

static void	add_token_with_quotes(t_token **tokens, int *count, char *str,
		int quote_type)
{
	t_token	*tmp;

	tmp = realloc(*tokens, sizeof(t_token) * (*count + 1));
	if (!tmp)
		exit(1);
	*tokens = tmp;
	(*tokens)[*count].str = ft_strdup(str);
	if (!(*tokens)[*count].str)
		exit(1);
	(*tokens)[*count].type = find_type((*tokens)[*count].str, *tokens, *count);
	(*tokens)[*count].quote_type = quote_type;
	(*count)++;
}

static void	add_token(t_token **tokens, int *count, char *str)
{
	add_token_with_quotes(tokens, count, str, 0);
}

t_token	*tokenisation(char *input, int *out_count)
{
	t_token	*tokens;
	char	*buf;
	int		count;
	int		i;
	char	c;
	int		sq;
	int		dq;
	int		quote_type;
	char	tmp_buf[3];

	i = 0;
	sq = 0;
	dq = 0;
	quote_type = 0;
	tokens = NULL;
	buf = ft_strdup("");
	count = 0;
	while (input[i])
	{
		c = input[i];
		if (c == '\'' && dq == 0)
		{
			if (!sq)
				quote_type = 1;
			sq = !sq;
			i++;
			continue ;
		}
		if (c == '"' && sq == 0)
		{
			if (!dq)
				quote_type = 2;
			dq = !dq;
			i++;
			continue ;
		}
		if (isspace((unsigned char)c) && sq == 0 && dq == 0)
		{
			if (ft_strlen(buf) > 0)
			{
				add_token_with_quotes(&tokens, &count, buf, quote_type);
				free(buf);
				buf = ft_strdup("");
				quote_type = 0;
			}
			i++;
			continue ;
		}
		if (is_special_char(c) && sq == 0 && dq == 0)
		{
			if (ft_strlen(buf) > 0)
			{
				add_token_with_quotes(&tokens, &count, buf, quote_type);
				free(buf);
				buf = ft_strdup("");
				quote_type = 0;
			}
			if ((c == '<' && input[i + 1] == '<') || (c == '>' && input[i
					+ 1] == '>'))
			{
				tmp_buf[0] = c;
				tmp_buf[1] = input[i + 1];
				tmp_buf[2] = '\0';
				add_token(&tokens, &count, tmp_buf);
				i += 2;
				continue ;
			}
			tmp_buf[0] = c;
			tmp_buf[1] = '\0';
			add_token(&tokens, &count, tmp_buf);
			i++;
			continue ;
		}
		append_char(&buf, c);
		i++;
	}
	if (ft_strlen(buf) > 0)
		add_token_with_quotes(&tokens, &count, buf, quote_type);
	free(buf);
	*out_count = count;
	return (tokens);
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
