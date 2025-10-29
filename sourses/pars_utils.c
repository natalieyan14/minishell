/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:37:12 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/30 00:02:28 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	free_redir_list(t_redir *head)
{
	t_redir	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->filename);
		free(tmp);
	}
}

void	free_ordered_redir_list(t_ordered_redir *head)
{
	t_ordered_redir	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->filename);
		free(tmp);
	}
}

t_ordered_redir	*add_ordered_redir(t_ordered_redir **head, t_redir_type type,
		char *filename, int order)
{
	t_ordered_redir	*new_redir;
	t_ordered_redir	*curr;

	new_redir = new_ordered_redir(type, filename, order);
	if (!new_redir)
		return (NULL);
	if (!*head)
	{
		*head = new_redir;
		return (new_redir);
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = new_redir;
	return (new_redir);
}
