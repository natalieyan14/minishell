/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:49:33 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/30 23:49:35 by natalieyan       ###   ########.fr       */
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
