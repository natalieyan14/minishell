/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 23:46:05 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/30 00:09:42 by natalieyan       ###   ########.fr       */
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

int	fill_command(t_command *cmd, t_token *tokens, int start, int end)
{
	int	j;
	int	ret;

	j = 0;
	while (start < end)
	{
		ret = 0;
		if (tokens[start].type == T_WORD)
			ret = handle_word_token(cmd, &tokens[start], &j);
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

int	handle_input_redir(t_command *cmd, t_token *token, int pos)
{
	if (!add_input_redir(&cmd->input_list, token->str))
		return (-1);
	if (!add_ordered_redir(&cmd->ordered_redirs, REDIR_INPUT, token->str, pos))
		return (-1);
	if (cmd->input)
		free(cmd->input);
	cmd->input = ft_strdup(token->str);
	return (0);
}
