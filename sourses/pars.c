/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:05:25 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/02 03:53:20 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	count_argc(t_token *tokens, int start, int end)
{
	int	count;
	int	i;

	count = 0;
	i = start;
	while (i < end)
	{
		if (tokens[i].type == T_WORD)
			count++;
		i++;
	}
	return (count);
}

static t_command	*create_command(t_token *tokens, int start, int end)
{
	t_command	*cmd;
	int			arg_count;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	init_command(cmd);
	arg_count = count_argc(tokens, start, end);
	cmd->argc = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->argc)
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

static t_command	*append_command(t_append_data *data)
{
	t_command	*cmd;

	cmd = create_command(data->tokens, data->start, data->end);
	if (!cmd)
		return (NULL);
	if (!*(data->head))
	{
		*(data->head) = cmd;
		*(data->curr) = cmd;
	}
	else
	{
		(*(data->curr))->next = cmd;
		*(data->curr) = cmd;
	}
	return (cmd);
}

t_command	*parse_tokens(t_token *tokens, int count)
{
	t_command		*head;
	t_command		*curr;
	int				i;
	int				start;
	t_append_data	data;

	head = NULL;
	curr = NULL;
	i = 0;
	while (i < count)
	{
		start = i;
		while (i < count && tokens[i].type != T_PIPE && tokens[i].type != T_OR
			&& tokens[i].type != T_AND)
			i++;
		data.curr = &curr;
		data.head = &head;
		data.tokens = tokens;
		data.start = start;
		data.end = i;
		if (!append_command(&data))
			return (NULL);
		if (i < count && (tokens[i].type == T_PIPE || tokens[i].type == T_OR
				|| tokens[i].type == T_AND))
			i++;
	}
	return (head);
}

t_ordered_redir	*new_ordered_redir(t_redir_type type, char *filename, int order)
{
	t_ordered_redir	*redir;

	redir = malloc(sizeof(t_ordered_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->filename = ft_strdup(filename);
	if (!redir->filename)
	{
		free(redir);
		return (NULL);
	}
	redir->order = order;
	redir->next = NULL;
	return (redir);
}
