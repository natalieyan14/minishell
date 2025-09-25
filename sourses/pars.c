/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 01:53:46 by natalieyan        #+#    #+#             */
/*   Updated: 2025/09/25 01:53:46 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	count_args(t_token *tokens, int start, int end)
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

static void	fill_command(t_command *cmd, t_token *tokens, int start, int end)
{
	int	j;

	j = 0;
	while (start < end)
	{
		if (tokens[start].type == T_WORD)
			cmd->args[j++] = ft_strdup(tokens[start].str);
		else if (tokens[start].type == T_IN_FILE)
			cmd->input = ft_strdup(tokens[start].str);
		else if (tokens[start].type == T_OUT_FILE)
		{
			cmd->output = ft_strdup(tokens[start].str);
			cmd->append = 0;
		}
		else if (tokens[start].type == T_APPEND_FILE)
		{
			cmd->output = ft_strdup(tokens[start].str);
			cmd->append = 1;
		}
		start++;
	}
	cmd->args[j] = NULL;
}

static t_command	*create_command(t_token *tokens, int start, int end)
{
	t_command	*cmd;
	int			arg_count;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	arg_count = count_args(tokens, start, end);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return (NULL);
	fill_command(cmd, tokens, start, end);
	return (cmd);
}

t_command	*parse_tokens(t_token *tokens, int count)
{
	t_command	*head;
	t_command	*curr;
	int			i;
	int			start;

	head = NULL;
	curr = NULL;
	i = 0;
	while (i < count)
	{
		start = i;
		while (i < count && tokens[i].type != T_PIPE)
			i++;
		if (!head)
			head = create_command(tokens, start, i);
		else
		{
			curr->next = create_command(tokens, start, i);
			curr = curr->next;
		}
		i++;
	}
	return (head);
}
