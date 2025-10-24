/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulitin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:13 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 02:57:30 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	ft_unset(t_env **env, char **argc)
{
	int		i;
	t_env	*tmp;
	t_env	*prev;

	i = 1;
	while (argc[i])
	{
		tmp = *env;
		prev = NULL;
		while (tmp)
		{
			if (!ft_strcmp(tmp->key, argc[i]))
			{
				if (!prev)
					*env = tmp->next;
				else
					prev->next = tmp->next;
				free(tmp->key);
				free(tmp->value);
				free(tmp);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
	SET_EXIT_STATUS(0);
}

int	is_builtin(t_command *cmd)
{
	if (!ft_strcmp(cmd->argc[0], "echo"))
		return (1);
	if (!ft_strcmp(cmd->argc[0], "cd"))
		return (1);
	if (!ft_strcmp(cmd->argc[0], "pwd"))
		return (1);
	if (!ft_strcmp(cmd->argc[0], "export"))
		return (1);
	if (!ft_strcmp(cmd->argc[0], "unset"))
		return (1);
	if (!ft_strcmp(cmd->argc[0], "env"))
		return (1);
	if (!ft_strcmp(cmd->argc[0], "exit"))
		return (1);
	return (0);
}

void	exec_builtin(t_command *cmd, t_env **env)
{
	if (!ft_strcmp(cmd->argc[0], "echo"))
		ft_echo(cmd->argc);
	else if (!ft_strcmp(cmd->argc[0], "cd"))
		ft_cd(cmd, *env);
	else if (!ft_strcmp(cmd->argc[0], "pwd"))
		ft_pwd();
	else if (!ft_strcmp(cmd->argc[0], "export"))
		ft_export(env, cmd->argc);
	else if (!ft_strcmp(cmd->argc[0], "unset"))
		ft_unset(env, cmd->argc);
	else if (!ft_strcmp(cmd->argc[0], "env"))
		ft_env(*env);
	else if (!ft_strcmp(cmd->argc[0], "exit"))
		ft_exit(cmd);
}
