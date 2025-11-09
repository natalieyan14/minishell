/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 22:33:35 by armtoros          #+#    #+#             */
/*   Updated: 2025/11/09 14:26:37 by natalieyan       ###   ########.fr       */
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
	if (arg_count >= 1)
	{
		if (!is_numeric(cmd->argc[1]))
			ft_exit_error(cmd->argc[1]);
		if (arg_count == 1)
		{
			status = ft_atoi(cmd->argc[1]);
			exit(status);
		}
	}
	ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
	set_exit_status(1);
	return ;
}

static int	update_existing_var(t_env *cur, char *val)
{
	char	*dup_val;

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

static char	*duplicate_value(char *val)
{
	if (!val)
		return (NULL);
	return (ft_strdup(val));
}

static int	init_env_strings(t_env *new, char *key, char *val)
{
	new->key = ft_strdup(key);
	if (!new->key)
		return (0);
	new->value = duplicate_value(val);
	if (val && !new->value)
	{
		free(new->key);
		return (0);
	}
	return (1);
}

static t_env	*create_new_env(char *key, char *val)
{
	t_env	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	if (!init_env_strings(new, key, val))
	{
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
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
