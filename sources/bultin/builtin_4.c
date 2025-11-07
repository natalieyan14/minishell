/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 22:33:35 by armtoros          #+#    #+#             */
/*   Updated: 2025/11/07 16:47:25 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	ft_exit_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	exit(255);
}

void	ft_exit(t_command *cmd)
{
	int	status;
	int	arg_count;

	arg_count = 0;
	while (cmd->argc[arg_count])
		arg_count++;
	arg_count--;
	if (isatty(STDIN_FILENO))
		write(1, "exit\n", 5);
	if (arg_count == 0)
		exit(get_exit_status() & 0xFF);
	if (arg_count == 1 && !is_numeric(cmd->argc[1]))
		ft_exit_error(cmd->argc[1]);
	if (arg_count == 1)
	{
		status = ft_atoi(cmd->argc[1]);
		exit(status & 0xFF);
	}
	ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
	set_exit_status(1);
	return ;
}

static void	update_existing_var(t_env *env, char *key, char *val)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			if (val)
			{
				free(env->value);
				env->value = ft_strdup(val);
			}
			return ;
		}
		env = env->next;
	}
}

void	export_variable(t_env **env, char *key, char *val)
{
	t_env	*tmp;
	t_env	*new_node;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			update_existing_var(*env, key, val);
			return ;
		}
		tmp = tmp->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = val ? ft_strdup(val) : NULL;
	new_node->next = *env;
	*env = new_node;
}

int	get_exit_status(void)
{
	return (get_current_exit_status());
}
