/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:54 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 02:04:55 by natalieyan       ###   ########.fr       */
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
	return (env_list);
}

char	**list_to_array(t_env *env_list)
{
	return (env_to_array(env_list));
}
