/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:00:05 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 17:00:08 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	alloc_argc(t_command *cmd, t_token *tokens, int start, int end)
{
	int	arg_count;

	arg_count = count_argc(tokens, start, end);
	cmd->argc = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->argc)
		return (-1);
	return (0);
}

t_command	*create_command(t_token *tokens, int start, int end)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	init_command(cmd);
	if (alloc_argc(cmd, tokens, start, end) < 0)
	{
		free(cmd);
		return (NULL);
	}
	if (fill_command(cmd, tokens, start, end) < 0)
	{
		free(cmd->argc);
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

int	handle_output_redir(t_command *cmd, t_token *token, int pos)
{
	if (!add_output_redir(&cmd->output_list, token->str, 0))
		return (-1);
	if (!add_ordered_redir(&cmd->ordered_redirs, REDIR_OUTPUT, token->str, pos))
		return (-1);
	return (0);
}

int	handle_append_redir(t_command *cmd, t_token *token, int pos)
{
	if (!add_output_redir(&cmd->output_list, token->str, 1))
		return (-1);
	if (!add_ordered_redir(&cmd->ordered_redirs, REDIR_APPEND, token->str, pos))
		return (-1);
	return (0);
}
