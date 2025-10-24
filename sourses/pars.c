/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:05:25 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 02:57:32 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	count_argc(t_token *tokens, int start, int end)
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

static int	validate_redirections(t_token *tokens, int start, int end)
{
	int	i;
	int	output_count;

	output_count = 0;
	i = start;
	while (i < end)
	{
		if (tokens[i].type == T_OUT_FILE || tokens[i].type == T_APPEND_FILE)
			output_count++;
		i++;
	}
	if (output_count <= 1)
		return (0);
	i = start;
	while (i < end)
	{
		if (tokens[i].type == T_OUT_FILE || tokens[i].type == T_APPEND_FILE)
		{
			if (access(tokens[i].str, F_OK) == 0 && access(tokens[i].str,
					W_OK) != 0)
			{
				perror(tokens[i].str);
				SET_EXIT_STATUS(1);
				return (-1);
			}
		}
		i++;
	}
	return (0);
}

static int	fill_command(t_command *cmd, t_token *tokens, int start, int end)
{
	int	j;

	if (validate_redirections(tokens, start, end) < 0)
		return (-1);
	j = 0;
	while (start < end)
	{
		if (tokens[start].type == T_WORD)
		{
			if (tokens[start].str && ft_strlen(tokens[start].str) > 0)
			{
				cmd->argc[j] = ft_strdup(tokens[start].str);
				if (cmd->argc[j])
					j++;
			}
		}
		else if (tokens[start].type == T_IN_FILE)
		{
			if (cmd->input)
				free(cmd->input);
			cmd->input = ft_strdup(tokens[start].str);
		}
		else if (tokens[start].type == T_OUT_FILE)
		{
			if (cmd->output)
				free(cmd->output);
			cmd->output = ft_strdup(tokens[start].str);
			cmd->append = 0;
		}
		else if (tokens[start].type == T_APPEND_FILE)
		{
			if (cmd->output)
				free(cmd->output);
			cmd->output = ft_strdup(tokens[start].str);
			cmd->append = 1;
		}
		start++;
	}
	cmd->argc[j] = NULL;
	return (0);
}

static t_command	*create_command(t_token *tokens, int start, int end)
{
	t_command	*cmd;
	int			arg_count;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argc = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->append = 0;
	cmd->next = NULL;
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
		{
			head = create_command(tokens, start, i);
			curr = head;
			if (!head)
				return (NULL);
		}
		else
		{
			curr->next = create_command(tokens, start, i);
			if (curr->next)
				curr = curr->next;
		}
		if (i < count && tokens[i].type == T_PIPE)
			i++;
	}
	return (head);
}
