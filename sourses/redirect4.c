/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:01:36 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 17:01:38 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header.h"

int	setup_ordered_redirections(t_command *cmd)
{
	if (!cmd->ordered_redirs)
		return (setup_redirections(cmd));
	if (validate_setup_helper(cmd->ordered_redirs) < 0)
		return (-1);
	if (setup_helper(cmd->ordered_redirs) < 0)
		return (-1);
	return (0);
}
