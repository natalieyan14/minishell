/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 22:33:35 by armtoros          #+#    #+#             */
/*   Updated: 2025/11/11 13:49:45 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

void	ft_exit_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	exit(2);
}

void	ft_exit(t_command *cmd)
{
	long	status;
	int		arg_count;

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
			status = ft_atol(cmd->argc[1]);
			exit((unsigned char)status);
		}
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
