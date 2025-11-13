/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:00:35 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 17:00:38 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_ordered_redir	*new_ordered_redir(t_redir_type type, char *filename, int order)
{
	t_ordered_redir	*redir;

	redir = malloc(sizeof(t_ordered_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->filename = ft_strdup(filename);
	if (!redir->filename)
	{
		free(redir);
		return (NULL);
	}
	redir->order = order;
	redir->next = NULL;
	return (redir);
}
