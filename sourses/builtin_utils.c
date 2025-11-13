/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 21:10:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/13 16:47:48 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	is_numeric(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	init_env_strings(t_env *new, char *key, char *val)
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

t_env	*create_new_env(char *key, char *val)
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

int	handle_too_many_args(t_command *cmd)
{
	if (cmd && cmd->argc && cmd->argc[1] && cmd->argc[2])
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		set_exit_status(1);
		return (1);
	}
	return (0);
}

long	ft_atol(const char *str)
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
