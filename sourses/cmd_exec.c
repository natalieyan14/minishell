/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:56:55 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 16:56:57 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	free_single_command(t_command *cmd)
{
	int	i;

	if (cmd->argc)
	{
		i = 0;
		while (cmd->argc[i])
		{
			free(cmd->argc[i]);
			i++;
		}
		free(cmd->argc);
	}
	if (cmd->input)
		free(cmd->input);
	if (cmd->input_list)
		free_input_redir_list(cmd->input_list);
	if (cmd->output_list)
		free_redir_list(cmd->output_list);
	if (cmd->ordered_redirs)
		free_ordered_redir_list(cmd->ordered_redirs);
}

void	free_cmd_list(t_command *cmd_list)
{
	t_command	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		free_single_command(cmd_list);
		free(cmd_list);
		cmd_list = tmp;
	}
}

void	exec_cmd_list(t_command *cmd_list, char **env)
{
	while (cmd_list)
	{
		if (cmd_list->argc && cmd_list->argc[0])
			exec_command(cmd_list->argc, env);
		cmd_list = cmd_list->next;
	}
}
