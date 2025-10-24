/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:28 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 03:23:59 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	free_cmd_list(t_command *cmd_list)
{
	t_command	*tmp;
	int			i;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		if (cmd_list->argc)
		{
			i = 0;
			while (cmd_list->argc[i])
			{
				free(cmd_list->argc[i]);
				i++;
			}
			free(cmd_list->argc);
		}
		if (cmd_list->input)
			free(cmd_list->input);
		if (cmd_list->output_list)
			free_redir_list(cmd_list->output_list);
		free(cmd_list);
		cmd_list = tmp;
	}
}

void	exec_cmd_list(t_command *cmd_list, char **env)
{
	while (cmd_list)
	{
		exec_command(cmd_list->argc, env);
		cmd_list = cmd_list->next;
	}
}
