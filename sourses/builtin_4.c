/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 22:33:35 by armtoros          #+#    #+#             */
/*   Updated: 2025/11/13 16:47:36 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	ft_exit_error(char *arg, t_env **env_list)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	clear_history();
	if (env_list && *env_list)
		free_env_list(*env_list);
	exit(2);
}

static void	do_exit_cleanup(int has_arg, char *arg, t_env **env_list)
{
	long	status;

	if (has_arg)
	{
		status = ft_atol(arg);
		clear_history();
		if (env_list && *env_list)
			free_env_list(*env_list);
		exit((unsigned char)status);
	}
	clear_history();
	if (env_list && *env_list)
		free_env_list(*env_list);
	exit(0);
}

void	ft_exit(t_command *cmd, t_env **env_list)
{
	int		arg_count;

	arg_count = 0;
	while (cmd->argc[arg_count])
		arg_count++;
	arg_count--;
	if (isatty(STDIN_FILENO))
		write(1, "exit\n", 5);
	if (arg_count == 0)
		do_exit_cleanup(0, NULL, env_list);
	if (arg_count >= 1)
	{
		if (!is_numeric(cmd->argc[1]))
			ft_exit_error(cmd->argc[1], env_list);
		if (arg_count == 1)
			do_exit_cleanup(1, cmd->argc[1], env_list);
	}
	ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
	set_exit_status(1);
	return ;
}

static void	add_env_to_list(t_env **env, t_env *new)
{
	t_env	*cur;

	if (*env == NULL)
	{
		*env = new;
		return ;
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

int	export_variable(t_env **env, char *key, char *val)
{
	t_env	*cur;
	t_env	*new;

	if (!key || !env)
		return (1);
	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
			return (update_existing_var(cur, val));
		cur = cur->next;
	}
	new = create_new_env(key, val);
	if (!new)
		return (1);
	add_env_to_list(env, new);
	return (0);
}
