/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:00:29 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/24 18:19:26 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	handle_word_token(t_command *cmd, t_token *token, int *j)
{
	if (token->str && ft_strlen(token->str) > 0)
	{
		cmd->argc[*j] = ft_strdup(token->str);
		if (cmd->argc[*j])
			(*j)++;
	}
	return (0);
}

static int	handle_redir_token(t_command *cmd, t_token *token, int pos)
{
	if (token->type == T_IN_FILE)
		return (handle_input_redir(cmd, token, pos));
	else if (token->type == T_OUT_FILE)
		return (handle_output_redir(cmd, token, pos));
	else if (token->type == T_APPEND_FILE)
		return (handle_append_redir(cmd, token, pos));
	return (0);
}

static int	join_prev_if_no_space(t_command *cmd, t_token *tokens, int start,
		int *j)
{
	char	*tmp;

	if (start > 0 && tokens[start - 1].type == T_WORD && tokens[start
			- 1].no_space && *j > 0 && cmd->argc[*j - 1])
	{
		tmp = ft_strjoin(cmd->argc[*j - 1], tokens[start].str);
		if (!tmp)
			return (-1);
		free(cmd->argc[*j - 1]);
		cmd->argc[*j - 1] = tmp;
		return (1);
	}
	return (0);
}

int	fill_command(t_command *cmd, t_token *tokens, int start, int end)
{
	int	j;
	int	ret;

	j = 0;
	while (start < end)
	{
		ret = 0;
		if (tokens[start].type == T_WORD)
		{
			ret = join_prev_if_no_space(cmd, tokens, start, &j);
			if (ret == 0)
				ret = handle_word_token(cmd, &tokens[start], &j);
		}
		else if (tokens[start].type == T_IN_FILE
			|| tokens[start].type == T_OUT_FILE
			|| tokens[start].type == T_APPEND_FILE)
			ret = handle_redir_token(cmd, &tokens[start], start);
		if (ret < 0)
			return (-1);
		start++;
	}
	cmd->argc[j] = NULL;
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
