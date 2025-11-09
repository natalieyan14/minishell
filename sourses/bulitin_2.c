/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulitin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:13 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/09 17:11:41 by natalieyan       ###   ########.fr       */
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

static void	unset_error(char *arg)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

static int	is_valid_unset_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	ft_unset(t_env **env, char **argc)
{
	int	i;
	int	exit_code;

	i = 1;
	exit_code = 0;
	if (!argc[1])
	{
		set_exit_status(0);
		return ;
	}
	while (argc[i])
	{
		if (!is_valid_unset_identifier(argc[i]))
		{
			unset_error(argc[i]);
			exit_code = 1;
		}
		else
			remove_env_var(env, argc[i]);
		i++;
	}
	set_exit_status(exit_code);
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
