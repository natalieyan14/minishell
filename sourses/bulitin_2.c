/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulitin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:13 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/09 21:10:59 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

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

void	print_exported_vars(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->value && ft_strlen(tmp->value) > 0)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(tmp->key, STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(tmp->value, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(tmp->key, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
}
