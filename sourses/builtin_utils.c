/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 21:10:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/09 21:31:09 by natalieyan       ###   ########.fr       */
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
