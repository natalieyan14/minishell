/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 21:30:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/09 21:24:10 by natalieyan       ###   ########.fr       */
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
