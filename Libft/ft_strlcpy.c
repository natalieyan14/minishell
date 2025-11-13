/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:07:28 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 17:07:30 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dsize)
{
	const char	*src_start;

	src_start = src;
	if (dsize > 0)
	{
		while (--dsize && *src)
		{
			*dst++ = *src++;
		}
		*dst = '\0';
	}
	while (*src++)
		;
	return (src - src_start - 1);
}
