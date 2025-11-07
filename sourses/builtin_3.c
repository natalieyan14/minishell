/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 21:52:42 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/08 01:18:10 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	process_export_arg(t_env **env, char *arg)
{
	char	*eq;
	char	*key;
	char	*val;
	int		ret;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (1);
	}
	eq = ft_strchr(arg, '=');
	if (eq)
	{
		key = ft_substr(arg, 0, eq - arg);
		val = eq + 1;
	}
	else
	{
		key = ft_strdup(arg);
		val = NULL;
	}
	if (!key)
		return (1);
	ret = export_variable(env, key, val);
	free(key);
	return (ret);
}

void	ft_export(t_env **env, char **argc)
{
	int	i;
	int	exit_code;

	i = 1;
	exit_code = 0;
	if (!argc[1])
	{
		print_exported_vars(*env);
		set_exit_status(0);
		return ;
	}
	while (argc[i])
	{
		if (process_export_arg(env, argc[i]))
			exit_code = 1;
		i++;
	}
	set_exit_status(exit_code);
}

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
