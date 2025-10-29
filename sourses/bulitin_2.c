/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulitin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:13 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/30 00:08:07 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	remove_env_var(t_env **env, char *key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			if (!prev)
				*env = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	ft_unset(t_env **env, char **argc)
{
	int	i;

	i = 1;
	while (argc[i])
	{
		remove_env_var(env, argc[i]);
		i++;
	}
	set_exit_status(0);
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
