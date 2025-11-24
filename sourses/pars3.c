/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:00:35 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/24 18:19:13 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

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
