/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:00:11 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 17:00:14 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static t_redir	*create_output_redir(char *filename, int append)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->filename = ft_strdup(filename);
	if (!new_redir->filename)
	{
		free(new_redir);
		return (NULL);
	}
	new_redir->append = append;
	new_redir->next = NULL;
	return (new_redir);
}

t_redir	*add_output_redir(t_redir **head, char *filename, int append)
{
	t_redir	*new_redir;
	t_redir	*curr;

	new_redir = create_output_redir(filename, append);
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

static t_input_redir	*create_input_redir(char *filename)
{
	t_input_redir	*redir;

	redir = malloc(sizeof(t_input_redir));
	if (!redir)
		return (NULL);
	redir->filename = ft_strdup(filename);
	if (!redir->filename)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

t_input_redir	*add_input_redir(t_input_redir **head, char *filename)
{
	t_input_redir	*new_redir;
	t_input_redir	*curr;

	new_redir = create_input_redir(filename);
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

void	free_input_redir_list(t_input_redir *head)
{
	t_input_redir	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->filename);
		free(tmp);
	}
}
