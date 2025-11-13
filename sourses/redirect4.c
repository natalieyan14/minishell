/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 01:50:33 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/11 14:06:20 by natalieyan       ###   ########.fr       */
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
