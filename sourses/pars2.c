/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:30:22 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/25 13:30:24 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	handle_word_token(t_command *cmd, t_token *token, int *idx)
{
	if (token->str && (ft_strlen(token->str) > 0 || token->quote_type != 0))
	{
		cmd->argc[*idx] = ft_strdup(token->str);
		if (!cmd->argc[*idx])
			return (-1);
		(*idx)++;
	}
	return (0);
}

static int	join_prev_if_no_space(t_command *cmd, t_token *tokens,
		int pos, int *idx)
{
	char	*tmp;

	if (pos > 0 && tokens[pos - 1].type == T_WORD
		&& tokens[pos - 1].no_space && tokens[pos - 1].str
		&& ft_strlen(tokens[pos - 1].str) > 0 && *idx > 0
		&& cmd->argc[*idx - 1])
	{
		tmp = ft_strjoin(cmd->argc[*idx - 1], tokens[pos].str);
		if (!tmp)
			return (-1);
		free(cmd->argc[*idx - 1]);
		cmd->argc[*idx - 1] = tmp;
		return (1);
	}
	return (0);
}

static char	*get_merged_start(t_token *tokens, int start)
{
	if (tokens[start].str)
		return (ft_strdup(tokens[start].str));
	else
		return (ft_strdup(""));
}

static char	*merge_adjacent_words(t_token *tokens, int start, int end,
		int *next_out)
{
	int		i;
	char	*merged;
	char	*tmp;

	i = start + 1;
	merged = get_merged_start(tokens, start);
	if (!merged)
		return (NULL);
	while (i < end && tokens[i].type == T_WORD
		&& (tokens[i].no_space || tokens[i - 1].no_space))
	{
		if (tokens[i].str)
			tmp = ft_strjoin(merged, tokens[i].str);
		else
			tmp = ft_strjoin(merged, "");
		if (!tmp)
			return (free(merged), NULL);
		free(merged);
		merged = tmp;
		i++;
	}
	*next_out = i;
	return (merged);
}

static int	handle_redirection(t_command *cmd, t_token *tokens,
		int start, int end)
{
	int		next;
	int		ret;
	char	*merged;
	t_token	tmp_tok;

	merged = merge_adjacent_words(tokens, start, end, &next);
	if (!merged)
		return (-1);
	tmp_tok.str = merged;
	tmp_tok.type = tokens[start].type;
	tmp_tok.quote_type = tokens[start].quote_type;
	tmp_tok.no_space = tokens[start].no_space;
	if (tmp_tok.type == T_IN_FILE)
		ret = handle_input_redir(cmd, &tmp_tok, start);
	else if (tmp_tok.type == T_OUT_FILE)
		ret = handle_output_redir(cmd, &tmp_tok, start);
	else if (tmp_tok.type == T_APPEND_FILE)
		ret = handle_append_redir(cmd, &tmp_tok, start);
	else
		ret = 0;
	free(merged);
	if (ret < 0)
		return (-1);
	return (next);
}

static int	process_redir_token(t_command *cmd, t_token *tokens,
		int *start, int end)
{
	int	next;

	next = handle_redirection(cmd, tokens, *start, end);
	if (next < 0)
		return (-1);
	*start = next - 1;
	return (0);
}

static int	process_word(t_command *cmd, t_token *tokens, int *start, int *j)
{
	int	ret;

	ret = join_prev_if_no_space(cmd, tokens, *start, j);
	if (ret == 0)
		ret = handle_word_token(cmd, &tokens[*start], j);
	return (ret);
}

static int	process_token(t_command *cmd, t_token *tokens, t_process_data *data)
{
	int	ret;

	ret = 0;
	if (tokens[data->start].type == T_WORD)
		ret = process_word(cmd, tokens, &data->start, &data->j);
	else if (tokens[data->start].type == T_IN_FILE
		|| tokens[data->start].type == T_OUT_FILE
		|| tokens[data->start].type == T_APPEND_FILE)
		ret = process_redir_token(cmd, tokens, &data->start, data->end);
	return (ret);
}

int	fill_command(t_command *cmd, t_token *tokens, int start, int end)
{
	t_process_data	data;
	int				ret;

	data.j = 0;
	data.start = start;
	data.end = end;
	while (data.start < data.end)
	{
		ret = process_token(cmd, tokens, &data);
		if (ret < 0)
			return (-1);
		data.start++;
	}
	cmd->argc[data.j] = NULL;
	return (0);
}

void	init_command(t_command *cmd)
{
	cmd->argc = NULL;
	cmd->input = NULL;
	cmd->input_list = NULL;
	cmd->output_list = NULL;
	cmd->ordered_redirs = NULL;
	cmd->next = NULL;
}
