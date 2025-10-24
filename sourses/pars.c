/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:05:25 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 03:23:58 by natalieyan       ###   ########.fr       */
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

t_redir	*add_output_redir(t_redir **head, char *filename, int append)
{
	t_redir	*new_redir;
	t_redir	*curr;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->filename = ft_strdup(filename);
	if (!new_redir->filename)
	{
		free(new_redir);
		return (NULL);
	}
	new_redir->append = append;
	new_redir->next = NULL;
	if (!*head)
		*head = new_redir;
	else
	{
		curr = *head;
		while (curr->next)
			curr = curr->next;
		curr->next = new_redir;
	}
	return (new_redir);
}

void	free_redir_list(t_redir *head)
{
	t_redir	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->filename);
		free(tmp);
	}
}

static int	fill_command(t_command *cmd, t_token *tokens, int start, int end)
{
	int	j;

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
			if (!add_output_redir(&cmd->output_list, tokens[start].str, 0))
				return (-1);
		}
		else if (tokens[start].type == T_APPEND_FILE)
		{
			if (!add_output_redir(&cmd->output_list, tokens[start].str, 1))
				return (-1);
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
	cmd->output_list = NULL;
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
