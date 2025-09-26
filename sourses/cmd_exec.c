/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:36:07 by natalieyan        #+#    #+#             */
/*   Updated: 2025/09/26 21:46:59 by natalieyan       ###   ########.fr       */
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
		if (cmd_list->output)
			free(cmd_list->output);
		free(cmd_list);
		cmd_list = tmp;
	}
}

void	exec_cmd_list(t_command *cmd_list, char **env)
{
	while (cmd_list)
	{
		exec_command(cmd_list->argc[0], env);
		cmd_list = cmd_list->next;
	}
}
