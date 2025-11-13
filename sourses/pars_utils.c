/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:00:17 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 17:00:20 by nharutyu         ###   ########.fr       */
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
