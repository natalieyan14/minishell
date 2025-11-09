/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 21:52:42 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/09 17:00:41 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

static int	parse_export_arg(char *arg, char **key, char **val)
{
	char	*eq;
	char	*plus_eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		plus_eq = ft_strnstr(arg, "+=", eq - arg + 1);
		if (plus_eq && plus_eq == eq - 1)
			*key = ft_substr(arg, 0, plus_eq - arg);
		else
			*key = ft_substr(arg, 0, eq - arg);
		*val = eq + 1;
	}
	else
	{
		*key = ft_strdup(arg);
		*val = NULL;
	}
	return (*key != NULL);
}

int	process_export_arg(t_env **env, char *arg)
{
	char	*key;
	char	*val;
	int		ret;

	if (!is_valid_identifier(arg))
	{
		export_error(arg);
		return (1);
	}
	if (!parse_export_arg(arg, &key, &val))
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
		{
			if (str[i] == '+' && str[i + 1] == '=')
				break ;
			return (0);
		}
		i++;
	}
	return (1);
}
