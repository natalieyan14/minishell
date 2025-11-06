/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 21:35:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/28 22:44:57 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	*get_exit_status_ptr(void)
{
	static int	exit_status = 0;

	return (&exit_status);
}

void	set_exit_status(int status)
{
	*get_exit_status_ptr() = status;
}

int	get_current_exit_status(void)
{
	return (*get_exit_status_ptr());
}
