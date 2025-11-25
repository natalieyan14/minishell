/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:45:28 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/25 13:46:44 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	process_redir_token(t_command *cmd, t_token *tokens, int *start, int end)
{
	int	next;

	next = handle_redirection(cmd, tokens, *start, end);
	if (next < 0)
		return (-1);
	*start = next - 1;
	return (0);
}

int	process_word(t_command *cmd, t_token *tokens, int *start, int *j)
{
	int	ret;

	ret = join_prev_if_no_space(cmd, tokens, *start, j);
	if (ret == 0)
		ret = handle_word_token(cmd, &tokens[*start], j);
	return (ret);
}

int	process_token(t_command *cmd, t_token *tokens, t_process_data *data)
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
