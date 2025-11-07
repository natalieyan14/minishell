/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 22:33:35 by armtoros          #+#    #+#             */
/*   Updated: 2025/11/08 01:17:55 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	ft_exit_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	exit(2);
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
		exit(0);
	if (arg_count == 1)
	{
		if (!is_numeric(cmd->argc[1]))
			ft_exit_error(cmd->argc[1]);
		status = ft_atoi(cmd->argc[1]);
		exit(status);
	}
	ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
	set_exit_status(1);
	return ;
}

int	export_variable(t_env **env, char *key, char *val)
{
	t_env	*cur;
	t_env	*new;
	char	*dup_key;
	char	*dup_val;

	if (!key || !env)
		return (1);
	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			dup_val = NULL;
			if (val)
			{
				dup_val = ft_strdup(val);
				if (!dup_val)
					return (1);
			}
			free(cur->value);
			cur->value = dup_val;
			return (0);
		}
		cur = cur->next;
	}
	new = malloc(sizeof(*new));
	if (!new)
		return (1);
	dup_key = ft_strdup(key);
	if (!dup_key)
	{
		free(new);
		return (1);
	}
	if (val)
	{
		dup_val = ft_strdup(val);
		if (!dup_val)
		{
			free(dup_key);
			free(new);
			return (1);
		}
	}
	else
		dup_val = NULL;
	new->key = dup_key;
	new->value = dup_val;
	new->next = NULL;
	if (*env == NULL)
	{
		*env = new;
		return (0);
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
	return (0);
}
