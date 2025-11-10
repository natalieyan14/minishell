/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:54 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/11 01:59:25 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	free_env_list(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list->next;
		free(env_list->key);
		free(env_list->value);
		free(env_list);
		env_list = tmp;
	}
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static void	increment_shlvl(t_env **env_list)
{
	char	*shlvl_str;
	int		shlvl_val;
	char	*new_shlvl;

	shlvl_str = get_env_value(*env_list, "SHLVL");
	if (shlvl_str)
	{
		shlvl_val = ft_atoi(shlvl_str);
		free(shlvl_str);
	}
	else
		shlvl_val = 0;
	shlvl_val++;
	new_shlvl = ft_itoa(shlvl_val);
	if (new_shlvl)
	{
		export_variable(env_list, "SHLVL", new_shlvl);
		free(new_shlvl);
	}
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		add_env_node(&env_list, envp[i]);
		i++;
	}
	increment_shlvl(&env_list);
	return (env_list);
}

void	update_underscore_var(t_env **env_list, char *cmd_path)
{
	if (!env_list || !*env_list || !cmd_path)
		return ;
	export_variable(env_list, "_", cmd_path);
}
