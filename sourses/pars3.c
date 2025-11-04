/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armtoros <armtoros@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 22:46:28 by armtoros          #+#    #+#             */
/*   Updated: 2025/11/03 22:46:52 by armtoros         ###   ########.fr       */
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
