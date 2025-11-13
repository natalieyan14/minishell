/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:58:20 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 16:58:23 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	update_existing_var(t_env *cur, char *val)
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

char	*duplicate_value(char *val)
{
	if (!val)
		return (NULL);
	return (ft_strdup(val));
}
